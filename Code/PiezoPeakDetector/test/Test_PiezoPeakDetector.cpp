#include <gtest/gtest.h>
#include <memory>
#include <tuple> // For std::ignore

#include "PiezoPeakDetector/PiezoPeakDetector.h"

class TestPiezoPeakDetector : public ::testing::Test {
public:
  TestPiezoPeakDetector()
      : m_piezoPeakDetector(std::make_unique<PiezoPeakDetector>()),
        m_settings(std::make_unique<PiezoPeakDetectorSettings>(
            PiezoPeakDetectorSettings{
                .scanTime_us = 1000,
                .maskTime_us = 500,
                .threshold = 200,
            })) {
    DefaultSetup();
  }

  void DefaultSetup() { m_piezoPeakDetector->SetSettings(*m_settings); }

  std::unique_ptr<PiezoPeakDetector> m_piezoPeakDetector;
  std::unique_ptr<PiezoPeakDetectorSettings> m_settings;
};

//==============================
// GetState()
//==============================

TEST_F(TestPiezoPeakDetector, test_GetState_InitialStateIsIdle) {
  EXPECT_EQ(m_piezoPeakDetector->GetState(), PiezoPeakDetectorState::IDLE);
}

//==============================
// SetSettings() and GetSettings()
//==============================

TEST_F(TestPiezoPeakDetector, test_SetAndGetSettings) {
  m_piezoPeakDetector.reset();
  m_piezoPeakDetector = std::make_unique<PiezoPeakDetector>();

  m_piezoPeakDetector->SetSettings(*m_settings);
  auto retrievedSettings = m_piezoPeakDetector->GetSettings();

  EXPECT_EQ(retrievedSettings, *m_settings);
}

//==============================
// ProcessSample()
//==============================

TEST_F(TestPiezoPeakDetector,
       test_ProcessSample_ReturnsZeroValueWhenIdleAndBelowThreshold) {
  PiezoSample sample{.timestamp_us = 20, .value = 10};
  auto result = m_piezoPeakDetector->ProcessSample(sample);
  auto expectedResult =
      PiezoSampleProcessingResult{.scanTimeElapsed = false, .maxValue = 0};

  EXPECT_EQ(result, expectedResult);
}

TEST_F(TestPiezoPeakDetector,
       test_ProcessSample_ReturnsMaxReadValueWhenIdleAndAboveThreshold) {
  PiezoSample sample{.timestamp_us = 20, .value = 300};
  auto result = m_piezoPeakDetector->ProcessSample(sample);
  auto expectedResult =
      PiezoSampleProcessingResult{.scanTimeElapsed = false, .maxValue = 300};

  EXPECT_EQ(result, expectedResult);
}

TEST_F(TestPiezoPeakDetector,
       test_ProcessSample_GoesToScanStateWhenIdleAndAboveThreshold) {
  PiezoSample sample{.timestamp_us = 20, .value = 300};

  std::ignore = m_piezoPeakDetector->ProcessSample(sample);
  EXPECT_EQ(m_piezoPeakDetector->GetState(), PiezoPeakDetectorState::SCAN);
}

TEST_F(TestPiezoPeakDetector,
       test_ProcessSample_GoesToMaskStateWhenScanTimeElapsed) {
  PiezoSample sample{.timestamp_us = 20, .value = 300};
  std::ignore = m_piezoPeakDetector->ProcessSample(sample);

  sample.timestamp_us += m_settings->scanTime_us;
  std::ignore = m_piezoPeakDetector->ProcessSample(sample);
  EXPECT_EQ(m_piezoPeakDetector->GetState(), PiezoPeakDetectorState::MASK);
}

TEST_F(
    TestPiezoPeakDetector,
    test_ProcessSample_StaysInScanStateWhenScanTimeNotElapsedAndUpdatesMaxValue) {
  auto sample1 = PiezoSample{.timestamp_us = 20, .value = 300};
  std::ignore = m_piezoPeakDetector->ProcessSample(sample1);

  auto sample2 = PiezoSample{.timestamp_us = sample1.timestamp_us +
                                             m_settings->scanTime_us - 1,
                             .value = 400};

  auto result = m_piezoPeakDetector->ProcessSample(sample2);
  auto expectedResult = PiezoSampleProcessingResult{.scanTimeElapsed = false,
                                                    .maxValue = sample2.value};

  EXPECT_EQ(result, expectedResult);
  EXPECT_EQ(m_piezoPeakDetector->GetState(), PiezoPeakDetectorState::SCAN);
}

TEST_F(
    TestPiezoPeakDetector,
    test_ProcessSample_StaysInScanStateWhenScanTimeNotElapsedAndKeepsPreviousMaxValue) {
  auto sample1 = PiezoSample{.timestamp_us = 20, .value = 300};
  std::ignore = m_piezoPeakDetector->ProcessSample(sample1);

  auto sample2 = PiezoSample{.timestamp_us = sample1.timestamp_us +
                                             m_settings->scanTime_us - 1,
                             .value = 250};

  auto result = m_piezoPeakDetector->ProcessSample(sample2);
  auto expectedResult = PiezoSampleProcessingResult{.scanTimeElapsed = false,
                                                    .maxValue = sample1.value};

  EXPECT_EQ(result, expectedResult);
  EXPECT_EQ(m_piezoPeakDetector->GetState(), PiezoPeakDetectorState::SCAN);
}

TEST_F(
    TestPiezoPeakDetector,
    test_ProcessSample_ReturnScanTimeElapsedWithoutValueUpdateWhenScanTimeElapsed) {
  auto sample1 = PiezoSample{.timestamp_us = 20, .value = 300};
  std::ignore = m_piezoPeakDetector->ProcessSample(sample1);

  // The 400 value is higher than the previous max value of 300, but since the
  // scan time has elapsed, it should not update the max value.
  auto sample2 = PiezoSample{.timestamp_us =
                                 sample1.timestamp_us + m_settings->scanTime_us,
                             .value = 400};

  auto result = m_piezoPeakDetector->ProcessSample(sample2);
  auto expectedResult = PiezoSampleProcessingResult{.scanTimeElapsed = true,
                                                    .maxValue = sample1.value};

  EXPECT_EQ(result, expectedResult);
  EXPECT_EQ(m_piezoPeakDetector->GetState(), PiezoPeakDetectorState::MASK);
}

TEST_F(TestPiezoPeakDetector,
       test_ProcessSample_ReturnScanTimeNotElapsedOnSecondUpdateInMask) {
  auto sample1 = PiezoSample{.timestamp_us = 20, .value = 300};
  std::ignore = m_piezoPeakDetector->ProcessSample(sample1);

  auto sample2 = PiezoSample{.timestamp_us =
                                 sample1.timestamp_us + m_settings->scanTime_us,
                             .value = 400};
  std::ignore = m_piezoPeakDetector->ProcessSample(sample2);

  auto sample3 = PiezoSample{.timestamp_us = sample1.timestamp_us +
                                             m_settings->scanTime_us + 1,
                             .value = 250};
  auto result = m_piezoPeakDetector->ProcessSample(sample3);

  auto expectedResult = PiezoSampleProcessingResult{.scanTimeElapsed = false,
                                                    .maxValue = sample1.value};

  EXPECT_EQ(result, expectedResult);
  EXPECT_EQ(m_piezoPeakDetector->GetState(), PiezoPeakDetectorState::MASK);
}

TEST_F(
    TestPiezoPeakDetector,
    test_ProcessSample_GoesToIdleStateAndReturnZeroResultAfterMaskTimeElapsed) {
  auto sample1 = PiezoSample{.timestamp_us = 20, .value = 300};
  std::ignore = m_piezoPeakDetector->ProcessSample(sample1);

  auto sample2 = PiezoSample{.timestamp_us =
                                 sample1.timestamp_us + m_settings->scanTime_us,
                             .value = 400};
  std::ignore = m_piezoPeakDetector->ProcessSample(sample2);

  auto sample3 = PiezoSample{.timestamp_us = sample1.timestamp_us +
                                             m_settings->scanTime_us +
                                             m_settings->maskTime_us,
                             .value = 250};
  auto result = m_piezoPeakDetector->ProcessSample(sample3);

  auto expectedResult =
      PiezoSampleProcessingResult{.scanTimeElapsed = false, .maxValue = 0};

  EXPECT_EQ(result, expectedResult);
  EXPECT_EQ(m_piezoPeakDetector->GetState(), PiezoPeakDetectorState::IDLE);
}
