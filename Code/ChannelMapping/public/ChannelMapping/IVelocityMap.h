#pragma once

#include <stdint.h>

class IVelocityMap {
public:
#ifndef __AVR__
  virtual ~IVelocityMap() = default;
#endif

  virtual auto MapVelocity(uint16_t velocity) const -> uint16_t = 0;
};