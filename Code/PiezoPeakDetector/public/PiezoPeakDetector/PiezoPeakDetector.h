#pragma once

#include "PiezoPeakDetector/IPiezoPeakDetector.h"
#include "PiezoPeakDetector/States/IState.h"
#include "PiezoPeakDetector/States/PiezoPeakDetectorState.h"
#include "PiezoPeakDetector/States/PiezoPeakDetectorStateContext.h"

#include "PiezoPeakDetector/States/State_Idle.h"
#include "PiezoPeakDetector/States/State_Mask.h"
#include "PiezoPeakDetector/States/State_Scan.h"

class PiezoPeakDetector : public IPiezoPeakDetector {
public:
  PiezoPeakDetector();
  ~PiezoPeakDetector();

  //=== IPiezoPeakDetector implementation ===//
  void SetSettings(const PiezoPeakDetectorSettings &settings) override;

  auto GetSettings() const -> PiezoPeakDetectorSettings override;

  auto
  ProcessSample(PiezoSample sample) -> PiezoSampleProcessingResult override;

  void Reset() override;
  //=== End of IPiezoPeakDetector implementation ===//

  auto GetState() const -> PiezoPeakDetectorState;
  auto GetContext() const -> const PiezoPeakDetectorStateContext &;

private:
  PiezoPeakDetectorStateContext m_context{};
  State_Idle m_state_idle;
  State_Scan m_state_scan;
  State_Mask m_state_mask;
  IState<PiezoPeakDetectorState> *m_currentState;
};
