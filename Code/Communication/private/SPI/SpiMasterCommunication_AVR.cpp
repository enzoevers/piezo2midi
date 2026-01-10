#include "Communication/SPI/SpiMasterCommunication_AVR.h"
#ifndef BUILD_TESTS
#include <avr/io.h>
#endif

//====================
// Public
//====================

SpiMasterCommunication_AVR::SpiMasterCommunication_AVR(SpiConfig config,
                                                       IDigitalIO &digitalIO)
    : m_spiConfig(config), m_digitalIO(digitalIO) {
  // Set the SPI IO modes
  m_digitalIO.SetMode(m_spiConfig.MOSI_pin, PIN_MODE::OUTPUT);
  m_digitalIO.SetMode(m_spiConfig.MISO_pin, PIN_MODE::INPUT);
  m_digitalIO.SetMode(m_spiConfig.SS_pin, PIN_MODE::OUTPUT);
  m_digitalIO.SetMode(m_spiConfig.SCLK_pin, PIN_MODE::OUTPUT);

  SetupSpiRegisters();
}

// http://www.rjhcoding.com/avrc-spi.php

void SpiMasterCommunication_AVR::SendBytes(
    [[maybe_unused]] const size_t size,
    [[maybe_unused]] const char *const bytesToSend,
    [[maybe_unused]] char *const responseBuffer) {}

//====================
// Private
//====================

void SpiMasterCommunication_AVR::SetupSpiRegisters() {

  const uint8_t clockDivisor = CalculateClockDivisor();

  // TODO: Set the clock divisor bits (SPR1 and SPR0) in SPCR based on the

  // TODO: Check DORD bit for MSB/LSB first
  SPCR = (1 << MSTR)    // Enable Master mode
         | (1 << SPE)   // Enable SPI
         | (0 << DORD); // MSB first

  SetSpiModeBits();
}

void SpiMasterCommunication_AVR::SetSpiModeBits() {
  switch (m_spiConfig.spiMode) {
  case SPI_MODE::MODE0:
    // CPOL = 0, CPHA = 0
    SPCR &= ~((1 << CPOL) | (1 << CPHA)); // Clear CPOL and CPHA bits
    break;
  case SPI_MODE::MODE1:
    // CPOL = 0, CPHA = 1
    SPCR &= ~(1 << CPOL); // Clear CPOL bit
    SPCR |= (1 << CPHA);  // Set CPHA bit
    break;
  case SPI_MODE::MODE2:
    // CPOL = 1, CPHA = 0
    SPCR |= (1 << CPOL);  // Set CPOL bit
    SPCR &= ~(1 << CPHA); // Clear CPHA bit
    break;
  case SPI_MODE::MODE3:
    // CPOL = 1, CPHA = 1
    SPCR |= (1 << CPOL) | (1 << CPHA); // Set CPOL and CPHA bits
    break;
  }
}

auto SpiMasterCommunication_AVR::CalculateClockDivisor() -> uint8_t {
  const uint32_t cpuClockHz =
      F_CPU; // F_CPU is defined in the AVR build environment
  return 0;
}
