#pragma once

#include "Interrupt/TypeErasedCallable.h"

class IAtomicOperation {
public:
#ifndef __AVR__
  virtual ~IAtomicOperation() = default;
#endif

  virtual void ExecuteAtomicOperation(TypeErasedCallable &operation) = 0;
};
