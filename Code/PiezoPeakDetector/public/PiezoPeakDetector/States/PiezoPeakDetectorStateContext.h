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

  bool operator==(const PiezoPeakDetectorStateContext &other) const {
    return m_settings == other.m_settings &&
           m_currentMaxValue == other.m_currentMaxValue &&
           m_scanEndTime_us == other.m_scanEndTime_us &&
           m_maskEndTime_us == other.m_maskEndTime_us &&
           m_currentSample == other.m_currentSample &&
           m_currentResult == other.m_currentResult;
  }

  void ResetState() {
    m_currentMaxValue = 0;
    m_scanEndTime_us = 0;
    m_maskEndTime_us = 0;
    m_currentSample = {};
    m_currentResult = {};
  }
};