#pragma once

#include "Interrupt/IGlobalInterruptControl.h"
#include <gmock/gmock.h>

class Mock_IGlobalInterruptControl : public IGlobalInterruptControl {
public:
  ~Mock_IGlobalInterruptControl() = default;

  MOCK_METHOD(void, EnableGlobalInterrupts, (), (const, override));
  MOCK_METHOD(void, DisableGlobalInterrupts, (), (const, override));
  MOCK_METHOD(bool, GlobalInterruptsEnabled, (), (const, override));
};