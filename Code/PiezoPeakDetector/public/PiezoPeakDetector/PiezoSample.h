#pragma once

#include <stdint.h>

struct PiezoSample {
  uint64_t timestamp_us{};
  uint16_t value{};

  bool operator==(const PiezoSample &other) const {
    return timestamp_us == other.timestamp_us && value == other.value;
  }
};
