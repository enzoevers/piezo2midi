#pragma once

#include "Communication/UART/IIOProxy_UART_AVR.h"
#include <gmock/gmock.h>

class Mock_IOProxy_UART_AVR : public IIOProxy_UART_AVR {
public:
  ~Mock_IOProxy_UART_AVR() = default;

  MOCK_METHOD(void, WriteRegister,
              (UART_AVR_RegisterEnum registerName, uint8_t newValue),
              (override));
  MOCK_METHOD(uint8_t, ReadRegister, (UART_AVR_RegisterEnum registerName),
              (override));
  MOCK_METHOD(uint8_t, ReadDefine, (UART_AVR_DefineEnum defineName),
              (override));
};