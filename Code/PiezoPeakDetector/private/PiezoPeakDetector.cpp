#include "PiezoPeakDetector/PiezoPeakDetector.h"

PiezoPeakDetector::PiezoPeakDetector()
    : m_context({}), m_state_idle(m_context), m_state_scan(m_context),
      m_state_mask(m_context), m_currentState(&m_state_idle) {
  m_currentState->Enter();
}

PiezoPeakDetector::~PiezoPeakDetector() {
  m_currentState->Exit();
  m_currentState = nullptr;
}

void PiezoPeakDetector::SetSettings(const PiezoPeakDetectorSettings &settings) {
  m_context.m_settings = settings;
}

auto PiezoPeakDetector::GetSettings() const -> PiezoPeakDetectorSettings {
  return m_context.m_settings;
}

auto PiezoPeakDetector::ProcessSample(PiezoSample sample)
    -> PiezoSampleProcessingResult {

  m_context.m_currentResult = {};
  m_context.m_currentSample = sample;

  auto returnedState = m_currentState->Do();
  if (GetState() != returnedState) {
    m_currentState->Exit();
    switch (returnedState) {
    case PiezoPeakDetectorState::IDLE:
      m_currentState = &m_state_idle;
      break;
    case PiezoPeakDetectorState::SCAN:
      m_currentState = &m_state_scan;
      break;
    case PiezoPeakDetectorState::MASK:
      m_currentState = &m_state_mask;
      break;
    default:
      break;
    }
    m_currentState->Enter();
  }

  return m_context.m_currentResult;
}

auto PiezoPeakDetector::GetState() const -> PiezoPeakDetectorState {
  return m_currentState->GetState();
}
