#pragma once

#include <stdint.h>

struct PiezoSample {
  uint64_t timestamp_us{};
  uint16_t value{};
};
