#pragma once

#include "Interrupt/AVR_iom328p/InterruptVectorEnum.h"
#include "Interrupt/InterruptRegistration.h"

extern InterruptRegistration<INTERRUPT_VECTORS_iom328p>
    g_interruptRegistration_AVR_iom328p;