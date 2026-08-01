#pragma once

#include "PiezoPeakDetector/States/IState.h"
#include <gmock/gmock.h>

template <typename stateEnum> class Mock_IState : public IState<stateEnum> {
public:
  MOCK_METHOD(stateEnum, GetState, (), (override));

  MOCK_METHOD(void, Enter, (), (override));
  MOCK_METHOD(stateEnum, Do, (), (override));
  MOCK_METHOD(void, Exit, (), (override));
};