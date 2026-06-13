#include "Interrupt/GlobalInterruptControl.h"
#include <avr/interrupt.h>

void GlobalInterruptControl::EnableGlobalInterrupts() const { sei(); }

void GlobalInterruptControl::DisableGlobalInterrupts() const { cli(); }

bool GlobalInterruptControl::GlobalInterruptsEnabled() const {
  return (SREG & (1 << SREG_I)) != 0;
}
