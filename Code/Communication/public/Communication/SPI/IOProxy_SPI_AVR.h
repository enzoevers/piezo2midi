#pragma once

#include "Communication/SPI/IIOProxy_SPI_AVR.h"

class IOProxy_SPI_AVR : public IIOProxy_SPI_AVR {
public:
  IOProxy_SPI_AVR(uint8_t spiId);
  ~IOProxy_SPI_AVR() = default;

  void WriteRegister(SPI_AVR_RegisterEnum registerName,
                     uint8_t newValue) override;
  auto ReadRegister(SPI_AVR_RegisterEnum registerName) -> uint8_t override;
  auto ReadDefine(SPI_AVR_DefineEnum defineName) -> uint8_t override;

private:
  const uint8_t m_spiId;
};
