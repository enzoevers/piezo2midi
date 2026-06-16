#include "PiezoPeakDetector/States/State_Mask.h"

State_Mask::State_Mask(PiezoPeakDetectorStateContext &context)
    : m_context(context) {}

auto State_Mask::GetState() -> PiezoPeakDetectorState { return m_ownState; }

void State_Mask::Enter() {
  // Implementation for entering the mask state
}

auto State_Mask::Do() -> PiezoPeakDetectorState {
  if (m_context.m_currentSample.timestamp_us >= m_context.m_maskEndTime_us) {
    return PiezoPeakDetectorState::IDLE;
  }
  m_context.m_currentResult.maxValue = m_context.m_currentMaxValue;
  return m_ownState;
}

void State_Mask::Exit() {
  // Implementation for exiting the mask state
}
