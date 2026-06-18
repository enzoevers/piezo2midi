#pragma once

#include <stdint.h>

struct PiezoSampleProcessingResult {
  bool scanTimeElapsed{};
  uint16_t maxValue{};

  bool operator==(const PiezoSampleProcessingResult &other) const {
    return scanTimeElapsed == other.scanTimeElapsed &&
           maxValue == other.maxValue;
  }
};