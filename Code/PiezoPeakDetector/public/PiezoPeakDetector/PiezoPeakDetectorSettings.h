#pragma once

#include <stdint.h>

struct PiezoPeakDetectorSettings {
  uint64_t scanTime_us{};
  uint64_t maskTime_us{};
  uint16_t threshold{};

  bool operator==(const PiezoPeakDetectorSettings &other) const {
    return scanTime_us == other.scanTime_us &&
           maskTime_us == other.maskTime_us && threshold == other.threshold;
  }
};
