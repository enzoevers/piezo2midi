#pragma once

#include "Interrupt/TypeErasedCallable.h"

template <typename InterruptVectorEnum> class IInterruptRegistration {
public:
#ifndef __AVR__
  virtual ~IInterruptRegistration() = default;
#endif

  virtual void SetInterruptHandler(InterruptVectorEnum interruptVector,
                                   TypeErasedCallable handler) = 0;
  virtual TypeErasedCallable
  GetInterruptHandler(InterruptVectorEnum interruptVector) = 0;
};