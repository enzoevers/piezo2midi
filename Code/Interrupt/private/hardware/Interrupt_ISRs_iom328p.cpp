#include "Interrupt/AVR_iom328p/InterruptRegistration.h"
#include <avr/interrupt.h>

InterruptRegistration<INTERRUPT_VECTORS_iom328p>
    g_interruptRegistration_AVR_iom328p;

ISR(TIMER0_COMPA_vect) {
  const auto handler = g_interruptRegistration_AVR_iom328p.GetInterruptHandler(
      INTERRUPT_VECTORS_iom328p::IV_TIMER0_COMPA);
  if (handler) {
    handler();
  }
}
