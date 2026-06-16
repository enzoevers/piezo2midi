#include "DigitalIO/DigitalIO_AVR.h"
#include "DigitalIO/IOProxy_DigitalIO_AVR.h"

#include "Communication/UART/IOProxy_UART_AVR.h"
#include "Communication/UART/UART_AVR.h"

#include "Communication/SPI/IOProxy_SPI_AVR.h"
#include "Communication/SPI/SPIMaster_AVR.h"

#include "ADC/MCP3008/MCP3008.h"

#include "Timing/IOProxy_Timing_8bit_AVR.h"
#include "Timing/Timing_8bit_AVR.h"

#include "Interrupt/AVR_iom328p/InterruptRegistration.h"
#include "Interrupt/AtomicOperation.h"
#include "Interrupt/GlobalInterruptControl.h"

#include "PiezoPeakDetector/PiezoPeakDetector.h"

#include <stddef.h>
#include <stdio.h>

int main() {
  IOProxy_DigitalIO_AVR ioProxy_digitialIO_bankD(BANK_LETTER::D);
  DigitalIO_AVR digitalIO_D(ioProxy_digitialIO_bankD);

  const uint8_t pinLED = 7;
  digitalIO_D.SetMode(pinLED, PIN_MODE::OUTPUT);

  IOProxy_UART_AVR ioProxy_uart0(0);
  UART_AVR uart0(ioProxy_uart0);

  IOProxy_DigitalIO_AVR ioProxy_digitialIO_bankB(BANK_LETTER::B);
  DigitalIO_AVR digitalIO_B(ioProxy_digitialIO_bankB);

  // For this test app it is assumed
  // that a loopback connection is made between MOSI and MISO pins.
  PinControl slavePins[] = {{digitalIO_B, 2}};
  SPIPinConfig spiPinConfig{.MOSI_pin = {digitalIO_B, 3},
                            .MISO_pin = {digitalIO_B, 4},
                            .SCLK_pin = {digitalIO_B, 5},
                            .SS_pins = slavePins,
                            .slaveCount = 1};
  IOProxy_SPI_AVR ioProxy_spi0(0);
  SPIMaster_AVR spiMaster0(ioProxy_spi0, spiPinConfig, SPI_MODE::MODE0,
                           2'000'000, SPI_DATAORDER::MSB_FIRST);

  MCP3008 mcp3008(spiMaster0);

  const int OCRAPin = 6; // OC0A pin is pin 6 on bank D on the ATmega328P
  digitalIO_D.SetMode(OCRAPin, PIN_MODE::OUTPUT);
  // Also uncomment the marking of coma0Mask in SetupTimer(...)

  IOProxy_Timing_8bit_AVR ioProxy_timer0(0);
  AtomicOperation atomicOperation;
  GlobalInterruptControl globalInterruptControl;
  Timing_8bit_AVR timing(ioProxy_timer0, atomicOperation,
                         globalInterruptControl);
  auto timer0CompaHandler = [&timing]() { timing.TimerInterruptHandler(); };
  g_interruptRegistration_AVR_iom328p.SetInterruptHandler(
      INTERRUPT_VECTORS_iom328p::IV_TIMER0_COMPA,
      TypeErasedCallable::Bind(timer0CompaHandler));
  timing.SetupTimer(1000);
  timing.StartTimer();

  PiezoPeakDetector piezoPeakDetector_ch0;
  PiezoPeakDetector piezoPeakDetector_ch1;
  PiezoPeakDetector piezoPeakDetector_ch7;

  PiezoPeakDetectorSettings settings{
      .scanTime_us = 1000, .maskTime_us = 500, .threshold = 200};

  piezoPeakDetector_ch0.SetSettings(settings);
  piezoPeakDetector_ch1.SetSettings(settings);
  piezoPeakDetector_ch7.SetSettings(settings);

  const char message[] =
      "Send 'l' to turn off the LED\n\rSend 'h' to turn on the LED\n\r";

  for (size_t i = 0; message[i] != '\0'; ++i) {
    uart0.SendByte(message[i]);
  }

  globalInterruptControl.EnableGlobalInterrupts();

  while (true) {
    if (uart0.ByteAvailable()) {
      const char charReceived = uart0.ReadByte_NonBlocking();

      char logMessage[50];
      sprintf(logMessage, "Received UART char: %c\n\r", charReceived);
      for (size_t i = 0; logMessage[i] != '\0'; ++i) {
        uart0.SendByte(logMessage[i]);
      }

      if (charReceived == 'l') {
        digitalIO_D.SetState(pinLED, PIN_STATE::LOW);
        timing.Delay_us(750'000);
        digitalIO_D.SetState(pinLED, PIN_STATE::HIGH);
        timing.Delay_us(500'000);
        digitalIO_D.SetState(pinLED, PIN_STATE::LOW);
      } else if (charReceived == 'h') {
        digitalIO_D.SetState(pinLED, PIN_STATE::HIGH);
        timing.Delay_us(750'000);
        digitalIO_D.SetState(pinLED, PIN_STATE::LOW);
        timing.Delay_us(500'000);
        digitalIO_D.SetState(pinLED, PIN_STATE::HIGH);
      }

      const auto currentTime_us =
          timing.GetInterruptCount() * timing.GetTimerPeriod_us();
      spiMaster0.DeselectSlave(0);
      spiMaster0.SelectSlave(0);
      const uint16_t adcValueChannel0 = mcp3008.ReadChannel(0);
      spiMaster0.DeselectSlave(0);
      spiMaster0.SelectSlave(0);
      const uint16_t adcValueChannel1 = mcp3008.ReadChannel(1);
      spiMaster0.DeselectSlave(0);
      spiMaster0.SelectSlave(0);
      const uint16_t adcValueChannel7 = mcp3008.ReadChannel(7);

      piezoPeakDetector_ch0.ProcessSample(PiezoSample{
          .timestamp_us = currentTime_us, .value = adcValueChannel0});
      piezoPeakDetector_ch1.ProcessSample(PiezoSample{
          .timestamp_us = currentTime_us, .value = adcValueChannel1});
      piezoPeakDetector_ch7.ProcessSample(PiezoSample{
          .timestamp_us = currentTime_us, .value = adcValueChannel7});

      const uint32_t vref_mv =
          5000; // Assume a 5V reference voltage for the ADC
      const uint32_t adcResolution = mcp3008.AdcResolution();
      const uint16_t voltageChannel0 =
          (static_cast<uint32_t>(adcValueChannel0) * vref_mv) / adcResolution;
      const uint16_t voltageChannel1 =
          (static_cast<uint32_t>(adcValueChannel1) * vref_mv) / adcResolution;
      const uint16_t voltageChannel7 =
          (static_cast<uint32_t>(adcValueChannel7) * vref_mv) / adcResolution;

      char adcMessage[80];
      sprintf(adcMessage,
              "ADC Channel 0: %umV\n\rADC Channel 1: %umV\n\rADC Channel 7: "
              "%umV\n\r",
              voltageChannel0, voltageChannel1, voltageChannel7);
      for (size_t i = 0; adcMessage[i] != '\0'; ++i) {
        uart0.SendByte(adcMessage[i]);
      }
    }
  }

  return 0;
}
