#pragma once

#include "Timing/IIOProxy_Timing_8bit_AVR.h"
#include <gmock/gmock.h>

class Mock_IOProxy_Timing_8bit_AVR : public IIOProxy_Timing_8bit_AVR {
public:
  ~Mock_IOProxy_Timing_8bit_AVR() = default;

  MOCK_METHOD(void, WriteRegister,
              (Timing_8bit_AVR_RegisterEnum registerName, uint8_t newValue),
              (override));
  MOCK_METHOD(uint8_t, ReadRegister,
              (Timing_8bit_AVR_RegisterEnum registerName), (override));
  MOCK_METHOD(uint8_t, ReadDefine, (Timing_8bit_AVR_DefineEnum defineName),
              (override));
};