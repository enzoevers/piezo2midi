#pragma once

#include "Interrupt/IGlobalInterruptControl.h"

class GlobalInterruptControl : public IGlobalInterruptControl {
public:
  void EnableGlobalInterrupts() const override;
  void DisableGlobalInterrupts() const override;
  bool GlobalInterruptsEnabled() const override;
};