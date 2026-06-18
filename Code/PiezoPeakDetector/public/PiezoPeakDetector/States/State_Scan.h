#pragma once

#include "PiezoPeakDetector/States/IState.h"
#include "PiezoPeakDetector/States/PiezoPeakDetectorState.h"
#include "PiezoPeakDetector/States/PiezoPeakDetectorStateContext.h"

class State_Scan : public IState<PiezoPeakDetectorState> {
public:
  State_Scan(PiezoPeakDetectorStateContext &context);
  ~State_Scan() = default;

  auto GetState() -> PiezoPeakDetectorState override;

  void Enter() override;
  auto Do() -> PiezoPeakDetectorState override;
  void Exit() override;

private:
  PiezoPeakDetectorStateContext &m_context;
  const PiezoPeakDetectorState m_ownState{PiezoPeakDetectorState::SCAN};
};
