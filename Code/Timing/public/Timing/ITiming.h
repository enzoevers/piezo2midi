#pragma once

#include <stdint.h>

class ITiming {
public:
#ifndef __AVR__
  virtual ~ITiming() = default;
#endif

  virtual void SetupTimer(uint16_t timerValue_us) = 0;
  virtual void StartTimer() = 0;
  virtual void StopTimer() = 0;

  virtual void Delay_us(uint32_t us) = 0;
};