#pragma once

#include "Communication/SPI/IIOProxy_SPI_AVR.h"
#include <gmock/gmock.h>

class Mock_IOProxy_SPI_AVR : public IIOProxy_SPI_AVR {
public:
  ~Mock_IOProxy_SPI_AVR() = default;

  MOCK_METHOD(void, WriteRegister,
              (SPI_AVR_RegisterEnum registerName, uint8_t newValue),
              (override));
  MOCK_METHOD(uint8_t, ReadRegister, (SPI_AVR_RegisterEnum registerName),
              (override));
  MOCK_METHOD(uint8_t, ReadDefine, (SPI_AVR_DefineEnum defineName), (override));
};