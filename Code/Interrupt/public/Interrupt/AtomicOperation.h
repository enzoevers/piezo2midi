#pragma once

#include "Interrupt/IAtomicOperation.h"

class AtomicOperation : public IAtomicOperation {
public:
  void ExecuteAtomicOperation(TypeErasedCallable &operation) override;
};