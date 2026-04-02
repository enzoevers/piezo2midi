#pragma once

#include "Communication/SPI/ISpiMasterCommunication.h"
#include "Communication/SPI/SpiConfig.h"
#include "DigitalIO/IDigitalIO.h"

class SpiMasterCommunication_AVR : public ISpiMasterCommunication {
public:
  SpiMasterCommunication_AVR(SpiConfig config, IDigitalIO &digitalIO);
  ~SpiMasterCommunication_AVR() = default;

  /*
   * This function will send the bytes in bytesToSend and write the response
   * into responseBuffer. The size parameter indicates how many bytes to send.
   * The responseBuffer should be at least as large as size to hold the
   * response. Calling this function will use the SPI configuration provided
   * during initialization. It will also set the SPI clock speed according to
   * the CLK_hz parameter in the SpiConfig struct.
   */
  void SendBytes(const size_t size, const char *const bytesToSend,
                 char *const responseBuffer) override;

private:
  void SetupSpiRegisters();
  auto CalculateClockDivisor() -> uint8_t;
  void SetSpiModeBits();

  SpiConfig m_spiConfig{};
  IDigitalIO &m_digitalIO;
};
