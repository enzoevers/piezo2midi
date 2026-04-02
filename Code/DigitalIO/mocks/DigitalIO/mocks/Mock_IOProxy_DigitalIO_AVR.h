#pragma once

#include "DigitalIO/IIOProxy_DigitalIO_AVR.h"
#include <gmock/gmock.h>

class Mock_IOProxy_DigitalIO_AVR : public IIOProxy_DigitalIO_AVR {
public:
  ~Mock_IOProxy_DigitalIO_AVR() = default;

  MOCK_METHOD(void, WriteRegister,
              (DigitalIO_AVR_RegisterEnum registerName, uint8_t newValue),
              (override));
  MOCK_METHOD(uint8_t, ReadRegister, (DigitalIO_AVR_RegisterEnum registerName),
              (override));
  MOCK_METHOD(void, ReadDefine, (DigitialIO_AVR_DefineEnum defineName),
              (override));
};