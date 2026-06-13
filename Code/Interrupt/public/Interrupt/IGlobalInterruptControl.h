#pragma once

class IGlobalInterruptControl {
public:
#ifndef __AVR__
  virtual ~IGlobalInterruptControl() = default;
#endif
  virtual void EnableGlobalInterrupts() const = 0;
  virtual void DisableGlobalInterrupts() const = 0;
  virtual bool GlobalInterruptsEnabled() const = 0;
};