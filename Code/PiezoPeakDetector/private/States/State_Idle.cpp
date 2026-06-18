#include "PiezoPeakDetector/States/State_Idle.h"

State_Idle::State_Idle(PiezoPeakDetectorStateContext &context)
    : m_context(context) {}

auto State_Idle::GetState() -> PiezoPeakDetectorState { return m_ownState; }

void State_Idle::Enter() {
  // Implementation for entering the idle state
}

auto State_Idle::Do() -> PiezoPeakDetectorState {
  if (m_context.m_currentSample.value < m_context.m_settings.threshold) {
    return m_ownState;
  }
  m_context.m_currentMaxValue = m_context.m_currentSample.value;
  m_context.m_currentResult.maxValue = m_context.m_currentSample.value;
  m_context.m_scanEndTime_us =
      m_context.m_currentSample.timestamp_us + m_context.m_settings.scanTime_us;
  m_context.m_maskEndTime_us =
      m_context.m_scanEndTime_us + m_context.m_settings.maskTime_us;
  return PiezoPeakDetectorState::SCAN;
}

void State_Idle::Exit() {
  // Implementation for exiting the idle state
}
