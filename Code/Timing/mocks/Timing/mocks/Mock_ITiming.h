#pragma once

#include <gmock/gmock.h>

#include "Timing/ITiming.h"

class Mock_ITiming : public ITiming {
public:
  ~Mock_ITiming() = default;

  MOCK_METHOD(void, SetupTimer, (uint16_t timerValue_us), (override));
  MOCK_METHOD(void, StartTimer, (), (override));
  MOCK_METHOD(void, StopTimer, (), (override));

  MOCK_METHOD(void, Delay_us, (uint32_t), (override));
};