#pragma once

#include "IOProxy/IOProxy.h"
#include <stdint.h>

enum class DigitalIO_AVR_RegisterEnum {
  REG_DDRx,
  REG_PORTx,
  REG_PINx,

  INVALID
};

enum class DigitialIO_AVR_DefineEnum { INVALID };

class IIOProxy_DigitalIO_AVR
    : public IOProxy<DigitalIO_AVR_RegisterEnum, uint8_t,
                     DigitialIO_AVR_DefineEnum, void> {
public:
#ifndef __AVR__
  virtual ~IIOProxy_DigitalIO_AVR() = default;
#endif
  virtual void WriteRegister(DigitalIO_AVR_RegisterEnum registerName,
                             uint8_t newValue) = 0;
  virtual auto
  ReadRegister(DigitalIO_AVR_RegisterEnum registerName) -> uint8_t = 0;
  virtual auto ReadDefine(DigitialIO_AVR_DefineEnum defineName) -> void = 0;
};
