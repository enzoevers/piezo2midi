#pragma once

#include "PiezoPeakDetector/PiezoPeakDetectorSettings.h"
#include "PiezoPeakDetector/PiezoSample.h"
#include "PiezoPeakDetector/PiezoSampleProcessingResult.h"

class IPiezoPeakDetector {
public:
#ifndef __AVR__
  virtual ~IPiezoPeakDetector() = default;
#endif

  virtual void SetSettings(const PiezoPeakDetectorSettings &settings) = 0;

  virtual auto GetSettings() const -> PiezoPeakDetectorSettings = 0;

  virtual auto
  ProcessSample(PiezoSample sample) -> PiezoSampleProcessingResult = 0;

  /**
   * @brief Resets the internal state of the peak detector.
   */
  virtual void Reset() = 0;
};