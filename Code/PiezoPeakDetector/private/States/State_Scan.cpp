#include "PiezoPeakDetector/States/State_Scan.h"

State_Scan::State_Scan(PiezoPeakDetectorStateContext &context)
    : m_context(context) {}

auto State_Scan::GetState() -> PiezoPeakDetectorState { return m_ownState; }

void State_Scan::Enter() {
  // Implementation for entering the scan state
}

auto State_Scan::Do() -> PiezoPeakDetectorState {
  m_context.m_currentResult = PiezoSampleProcessingResult{
      .scanTimeElapsed = false, .maxValue = m_context.m_currentMaxValue};

  if (m_context.m_currentSample.timestamp_us >= m_context.m_scanEndTime_us) {
    m_context.m_currentResult.scanTimeElapsed = true;
    return PiezoPeakDetectorState::MASK;
  }

  if (m_context.m_currentSample.value > m_context.m_currentMaxValue) {
    m_context.m_currentMaxValue = m_context.m_currentSample.value;
    m_context.m_currentResult.maxValue = m_context.m_currentSample.value;
  }

  return m_ownState;
}

void State_Scan::Exit() {
  // Implementation for exiting the scan state
}
