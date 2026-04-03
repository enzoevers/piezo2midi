#pragma once

#include "IOProxy/IOProxy.h"
#include <stdint.h>

enum class SPI_AVR_RegisterEnum {
  REG_SPCRx,
  REG_SPDRx,
  REG_SPSRx,

  INVALID
};

enum class SPI_AVR_DefineEnum {
  // SPCRx
  DEF_SPIE,
  DEF_SPE,
  DEF_DORD,
  DEF_MSTR,
  DEF_CPOL,
  DEF_CPHA,
  DEF_SPR1,
  DEF_SPR0,

  // SPSRx
  DEF_SPIF,
  DEF_WCOL,
  DEF_SPI2X,

  INVALID
};

class IIOProxy_SPI_AVR : public IOProxy<SPI_AVR_RegisterEnum, uint8_t,
                                        SPI_AVR_DefineEnum, uint8_t> {
public:
#ifndef __AVR__
  virtual ~IIOProxy_SPI_AVR() = default;
#endif
  virtual void WriteRegister(SPI_AVR_RegisterEnum registerName,
                             uint8_t newValue) = 0;
  virtual auto ReadRegister(SPI_AVR_RegisterEnum registerName) -> uint8_t = 0;
  virtual auto ReadDefine(SPI_AVR_DefineEnum defineName) -> uint8_t = 0;
};
