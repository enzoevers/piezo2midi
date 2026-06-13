#pragma once

#include "Timing/IIOProxy_Timing_8bit_AVR.h"

class IOProxy_Timing_8bit_AVR : public IIOProxy_Timing_8bit_AVR {
public:
  IOProxy_Timing_8bit_AVR(uint8_t timerId);
  ~IOProxy_Timing_8bit_AVR() = default;

  void WriteRegister(Timing_8bit_AVR_RegisterEnum registerName,
                     uint8_t newValue) override;
  auto
  ReadRegister(Timing_8bit_AVR_RegisterEnum registerName) -> uint8_t override;
  auto ReadDefine(Timing_8bit_AVR_DefineEnum defineName) -> uint8_t override;

private:
  const uint8_t m_timerId;
};
