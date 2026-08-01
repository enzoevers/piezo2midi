#include <stdint.h>

// Generic IO related includes
#include "DigitalIO/DigitalIO_AVR.h"
#include "DigitalIO/IOProxy_DigitalIO_AVR.h"

// Channel processing related includes
#include "ChannelMapping/MidiChannel.h"
#include "ChannelMapping/VelocityMap.h"
#include "PiezoPeakDetector/PiezoPeakDetector.h"

// ADC Related includes
#include "ADC/MCP3008/MCP3008.h"
#include "Communication/SPI/IOProxy_SPI_AVR.h"
#include "Communication/SPI/SPIMaster_AVR.h"

// MIDI Related includes
#include "Communication/UART/IOProxy_UART_AVR.h"
#include "Communication/UART/UART_AVR.h"
#include "MIDI/V1_0/MIDI_V1_0.h"

// Timing related includes
#include "Interrupt/AVR_iom328p/InterruptRegistration.h"
#include "Interrupt/AtomicOperation.h"
#include "Interrupt/GlobalInterruptControl.h"
#include "Timing/IOProxy_Timing_8bit_AVR.h"
#include "Timing/Timing_8bit_AVR.h"

static constexpr uint8_t NUM_CHANNELS = 8;

uint8_t MapUint16ToMidiRange(uint16_t value) {
  static constexpr float scalingFactor = 127.f / UINT16_MAX;
  return static_cast<uint8_t>(value * scalingFactor);
}

void SampleChannel(MidiChannel &channel, PiezoSample &sample, MIDI_V1_0 &midi) {
  PiezoSampleProcessingResult result =
      channel.piezoPeakDetector->ProcessSample(sample);

  if (result.scanTimeElapsed) {
    uint16_t mappedVelocity = channel.velocityMap->MapVelocity(result.maxValue);
    uint8_t midiVelocity = MapUint16ToMidiRange(mappedVelocity);
    midi.SendStatusByte(MIDI_V1_0_STATUS::NOTE_ON, channel.midiChannel);
    midi.SendDataByte(midiVelocity);
  }
}

int main() {
  //==============================
  // Setup of objects
  //==============================

  // Channel processing setup
  MidiChannel midiChannels[NUM_CHANNELS];
  PiezoPeakDetector piezoPeakDetectors[NUM_CHANNELS];
  VelocityMap velocityMaps[NUM_CHANNELS] = {
      VelocityMap(UINT16_MAX, 1.0f), VelocityMap(UINT16_MAX, 1.0f),
      VelocityMap(UINT16_MAX, 1.0f), VelocityMap(UINT16_MAX, 1.0f),
      VelocityMap(UINT16_MAX, 1.0f), VelocityMap(UINT16_MAX, 1.0f),
      VelocityMap(UINT16_MAX, 1.0f), VelocityMap(UINT16_MAX, 1.0f)};

  for (uint8_t i = 0; i < NUM_CHANNELS; ++i) {
    piezoPeakDetectors[i].SetSettings(PiezoPeakDetectorSettings{
        .scanTime_us = 2'000, .maskTime_us = 6'000, .threshold = 1'000});

    midiChannels[i].adcChannel = i;
    midiChannels[i].midiChannel = static_cast<MIDI_V1_0_CHANNEL>(i);
    midiChannels[i].piezoPeakDetector = &piezoPeakDetectors[i];
    midiChannels[i].velocityMap = &velocityMaps[i];
  }

  // ADC setup
  IOProxy_DigitalIO_AVR ioProxy_digitialIO_bankB(BANK_LETTER::B);
  DigitalIO_AVR digitalIO_B(ioProxy_digitialIO_bankB);

  IOProxy_SPI_AVR ioProxy_spi0(0);
  PinControl slavePins[] = {{digitalIO_B, 2}};
  SPIPinConfig spiPinConfig{.MOSI_pin = {digitalIO_B, 3},
                            .MISO_pin = {digitalIO_B, 4},
                            .SCLK_pin = {digitalIO_B, 5},
                            .SS_pins = slavePins,
                            .slaveCount = 1};
  SPIMaster_AVR spiMaster0(ioProxy_spi0, spiPinConfig, SPI_MODE::MODE0,
                           2'000'000, SPI_DATAORDER::MSB_FIRST);
  MCP3008 mcp3008(spiMaster0);

  // MIDI setup
  IOProxy_UART_AVR ioProxy_uart0(0);
  UART_AVR uart0(ioProxy_uart0);

  MIDI_V1_0 midi(uart0);

  // Setup interrupts for the ADC sampling
  IOProxy_Timing_8bit_AVR ioProxy_timer0(0);
  AtomicOperation atomicOperation;
  GlobalInterruptControl globalInterruptControl;
  Timing_8bit_AVR timing(ioProxy_timer0, atomicOperation,
                         globalInterruptControl);

  auto timer0CompaHandler = [&midiChannels, &spiMaster0, &mcp3008, &midi,
                             &timing, &globalInterruptControl]() {
    globalInterruptControl.DisableGlobalInterrupts();

    timing.TimerInterruptHandler();
    const auto currentTime_us =
        timing.GetInterruptCount() * timing.GetTimerPeriod_us();

    PiezoSampleProcessingResult results[NUM_CHANNELS];
    for (uint8_t i = 0; i < NUM_CHANNELS; ++i) {
      spiMaster0.DeselectSlave(0);
      spiMaster0.SelectSlave(0);
      const uint16_t adcValue = mcp3008.ReadChannel(midiChannels[i].adcChannel);

      PiezoSample sample{.timestamp_us = currentTime_us, .value = adcValue};
      results[i] = midiChannels[i].piezoPeakDetector->ProcessSample(sample);
    }

    bool anyScanTimeElapsed = false;
    for (uint8_t i = 0; i < NUM_CHANNELS; ++i) {
      const auto &result = results[i];

      if (result.scanTimeElapsed) {
        anyScanTimeElapsed = true;
        uint16_t mappedVelocity =
            midiChannels[i].velocityMap->MapVelocity(result.maxValue);
        uint8_t midiVelocity = MapUint16ToMidiRange(mappedVelocity);
        midi.SendStatusByte(MIDI_V1_0_STATUS::NOTE_ON,
                            midiChannels[i].midiChannel);
        midi.SendDataByte(midiVelocity);
      }
    }

    if (anyScanTimeElapsed) {
      for (auto &channel : midiChannels) {
        channel.piezoPeakDetector->Reset();
      }
    }

    globalInterruptControl.EnableGlobalInterrupts();
  };

  g_interruptRegistration_AVR_iom328p.SetInterruptHandler(
      INTERRUPT_VECTORS_iom328p::IV_TIMER0_COMPA,
      TypeErasedCallable::Bind(timer0CompaHandler));

  // Sample the channels at a rate of 4kHz, which corresponds to a timer period
  // of 250us.
  timing.SetupTimer(250);
  timing.StartTimer();

  return 0;
}
