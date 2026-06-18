#pragma once

#include "PiezoPeakDetector/PiezoPeakDetectorSettings.h"
#include "PiezoPeakDetector/PiezoSample.h"
#include "PiezoPeakDetector/PiezoSampleProcessingResult.h"

struct PiezoPeakDetectorStateContext {
  PiezoPeakDetectorSettings m_settings{};
  uint16_t m_currentMaxValue{};
  uint64_t m_scanEndTime_us{};
  uint64_t m_maskEndTime_us{};
  PiezoSample m_currentSample{};
  PiezoSampleProcessingResult m_currentResult{};
};