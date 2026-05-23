#pragma once

#include "Interrupt/IInterruptRegistration.h"
#include <gmock/gmock.h>

template <typename InterruptVectorEnum>
class Mock_IInterruptRegistration
    : public IInterruptRegistration<InterruptVectorEnum> {
public:
  ~Mock_IInterruptRegistration() = default;

  MOCK_METHOD(void, SetInterruptHandler,
              (InterruptVectorEnum, TypeErasedCallable), (override));
  MOCK_METHOD(TypeErasedCallable, GetInterruptHandler, (InterruptVectorEnum),
              (override));
};