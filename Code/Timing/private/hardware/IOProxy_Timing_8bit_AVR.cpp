#include "Timing/IOProxy_Timing_8bit_AVR.h"
#include "IOProxy_Timing_8bit_AVR_PeripheralMap.h"

IOProxy_Timing_8bit_AVR::IOProxy_Timing_8bit_AVR(uint8_t timerId)
    : m_timerId(timerId) {}

void IOProxy_Timing_8bit_AVR::WriteRegister(
    Timing_8bit_AVR_RegisterEnum registerName, uint8_t newValue) {
  switch (registerName) {
  case Timing_8bit_AVR_RegisterEnum::REG_TCCRxA:
    if (auto *reg = GetTCCRAddress(m_timerId)) {
      *reg = newValue;
    }
    break;
  case Timing_8bit_AVR_RegisterEnum::REG_TCCRxB:
    if (auto *reg = GetTCCRBAddress(m_timerId)) {
      *reg = newValue;
    }
    break;
  case Timing_8bit_AVR_RegisterEnum::REG_TCNTx:
    if (auto *reg = GetTCNTAddress(m_timerId)) {
      *reg = newValue;
    }
    break;
  case Timing_8bit_AVR_RegisterEnum::REG_OCRxA:
    if (auto *reg = GetOCRAAddress(m_timerId)) {
      *reg = newValue;
    }
    break;
  case Timing_8bit_AVR_RegisterEnum::REG_OCRxB:
    if (auto *reg = GetOCRBAddress(m_timerId)) {
      *reg = newValue;
    }
    break;
  case Timing_8bit_AVR_RegisterEnum::REG_TIMSKx:
    if (auto *reg = GetTIMSKAddress(m_timerId)) {
      *reg = newValue;
    }
    break;
  case Timing_8bit_AVR_RegisterEnum::REG_TIFRx:
    if (auto *reg = GetTIFRAddress(m_timerId)) {
      *reg = newValue;
    }
    break;

  default:
    break;
  }
}

auto IOProxy_Timing_8bit_AVR::ReadRegister(
    Timing_8bit_AVR_RegisterEnum registerName) -> uint8_t {
  switch (registerName) {
  case Timing_8bit_AVR_RegisterEnum::REG_TCCRxA: {
    auto const *const reg = GetTCCRAddress(m_timerId);
    return reg ? *reg : 0;
  }
  case Timing_8bit_AVR_RegisterEnum::REG_TCCRxB: {
    auto const *const reg = GetTCCRBAddress(m_timerId);
    return reg ? *reg : 0;
  }
  case Timing_8bit_AVR_RegisterEnum::REG_TCNTx: {
    auto const *const reg = GetTCNTAddress(m_timerId);
    return reg ? *reg : 0;
  }
  case Timing_8bit_AVR_RegisterEnum::REG_OCRxA: {
    auto const *const reg = GetOCRAAddress(m_timerId);
    return reg ? *reg : 0;
  }
  case Timing_8bit_AVR_RegisterEnum::REG_OCRxB: {
    auto const *const reg = GetOCRBAddress(m_timerId);
    return reg ? *reg : 0;
  }
  case Timing_8bit_AVR_RegisterEnum::REG_TIMSKx: {
    auto const *const reg = GetTIMSKAddress(m_timerId);
    return reg ? *reg : 0;
  }
  case Timing_8bit_AVR_RegisterEnum::REG_TIFRx: {
    auto const *const reg = GetTIFRAddress(m_timerId);
    return reg ? *reg : 0;
  }
  default:
    return 0;
  }
}

auto IOProxy_Timing_8bit_AVR::ReadDefine(Timing_8bit_AVR_DefineEnum defineName)
    -> uint8_t {
  switch (defineName) {

  // TCCRxA
  case Timing_8bit_AVR_DefineEnum::DEF_COMxA1:
    return GetCOMA1Value(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_COMxA0:
    return GetCOMA0Value(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_COMxB1:
    return GetCOMB1Value(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_COMxB0:
    return GetCOMB0Value(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_WGMx1:
    return GetWGM1Value(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_WGMx0:
    return GetWGM0Value(m_timerId);

  // TCCRxB
  case Timing_8bit_AVR_DefineEnum::DEF_FOCxA:
    return GetFOCAValue(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_FOCxB:
    return GetFOCBValue(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_WGMx2:
    return GetWGM2Value(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_CSx2:
    return GetCS2Value(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_CSx1:
    return GetCS1Value(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_CSx0:
    return GetCS0Value(m_timerId);

  // TIMSKx
  case Timing_8bit_AVR_DefineEnum::DEF_OCIExB:
    return GetOCIEBValue(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_OCIExA:
    return GetOCIEAValue(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_TOIEx:
    return GetTOIEValue(m_timerId);

  // TIFRx
  case Timing_8bit_AVR_DefineEnum::DEF_OCFxB:
    return GetOCFBValue(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_OCFxA:
    return GetOCFAValue(m_timerId);
  case Timing_8bit_AVR_DefineEnum::DEF_TOVx:
    return GetTOVValue(m_timerId);
  default:
    return 0;
  }
}