#pragma once

#include "PiezoPeakDetector/IPiezoPeakDetector.h"
#include <gmock/gmock.h>

class Mock_IPiezoPeakDetector : public IPiezoPeakDetector {
public:
  MOCK_METHOD(void, SetSettings, (const PiezoPeakDetectorSettings &settings),
              (override));
  MOCK_METHOD(PiezoPeakDetectorSettings, GetSettings, (), (const, override));
  MOCK_METHOD(PiezoSampleProcessingResult, ProcessSample, (PiezoSample sample),
              (override));
};