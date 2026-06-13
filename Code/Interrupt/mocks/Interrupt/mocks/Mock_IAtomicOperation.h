#pragma once

#include "Interrupt/IAtomicOperation.h"
#include <gmock/gmock.h>

class Mock_IAtomicOperation : public IAtomicOperation {
public:
  Mock_IAtomicOperation() {
    ON_CALL(*this, ExecuteAtomicOperation(testing::_))
        .WillByDefault([](TypeErasedCallable &operation) { operation(); });
  }
  ~Mock_IAtomicOperation() = default;

  MOCK_METHOD(void, ExecuteAtomicOperation, (TypeErasedCallable & operation),
              (override));
};