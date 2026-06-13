#pragma once

#include "IOProxy/IOProxy.h"
#include <stdint.h>

enum class Timing_8bit_AVR_RegisterEnum {
  REG_TCCRxA,
  REG_TCCRxB,
  REG_TCNTx,
  REG_OCRxA,
  REG_OCRxB,
  REG_TIMSKx,
  REG_TIFRx,

  INVALID
};

enum class Timing_8bit_AVR_DefineEnum {
  // TCCRxA
  DEF_COMxA1,
  DEF_COMxA0,
  DEF_COMxB1,
  DEF_COMxB0,
  DEF_WGMx1,
  DEF_WGMx0,

  // TCCRxB
  DEF_FOCxA,
  DEF_FOCxB,
  DEF_WGMx2,
  DEF_CSx2,
  DEF_CSx1,
  DEF_CSx0,

  // TIMSKx
  DEF_OCIExB,
  DEF_OCIExA,
  DEF_TOIEx,

  // TIFRx
  DEF_OCFxB,
  DEF_OCFxA,
  DEF_TOVx,

  INVALID
};

class IIOProxy_Timing_8bit_AVR
    : public IOProxy<Timing_8bit_AVR_RegisterEnum, uint8_t,
                     Timing_8bit_AVR_DefineEnum, uint8_t> {
public:
#ifndef __AVR__
  virtual ~IIOProxy_Timing_8bit_AVR() = default;
#endif

  virtual void WriteRegister(Timing_8bit_AVR_RegisterEnum registerName,
                             uint8_t newValue) = 0;
  virtual auto
  ReadRegister(Timing_8bit_AVR_RegisterEnum registerName) -> uint8_t = 0;
  virtual auto ReadDefine(Timing_8bit_AVR_DefineEnum defineName) -> uint8_t = 0;
};