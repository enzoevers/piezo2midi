#pragma once

#include "Communication/UART/IIOProxy_UART_AVR.h"

class IOProxy_UART_AVR : public IIOProxy_UART_AVR {
public:
  IOProxy_UART_AVR(uint8_t uartId);
  ~IOProxy_UART_AVR() = default;

  void WriteRegister(UART_AVR_RegisterEnum registerName,
                     uint8_t newValue) override;
  auto ReadRegister(UART_AVR_RegisterEnum registerName) -> uint8_t override;
  auto ReadDefine(UART_AVR_DefineEnum defineName) -> uint8_t override;

private:
  const uint8_t m_uartId;
};
