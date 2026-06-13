#pragma once

#include "Interrupt/AVR_iom328p/InterruptVectorEnum.h"
#include "Interrupt/IInterruptRegistration.h"

#include <stddef.h>

template <typename InterruptVectorEnum>
  requires requires { static_cast<size_t>(InterruptVectorEnum::SIZE); }
class InterruptRegistration
    : public IInterruptRegistration<InterruptVectorEnum> {
public:
  bool is_vector_valid(InterruptVectorEnum interruptVector) const {
    const auto index = static_cast<size_t>(interruptVector);
    return index < static_cast<size_t>(InterruptVectorEnum::SIZE);
  }

  void SetInterruptHandler(InterruptVectorEnum interruptVector,
                           TypeErasedCallable handler) override {
    if (!is_vector_valid(interruptVector)) {
      return;
    }
    m_interruptHandlers[static_cast<size_t>(interruptVector)] = handler;
  }

  TypeErasedCallable
  GetInterruptHandler(InterruptVectorEnum interruptVector) override {
    if (!is_vector_valid(interruptVector)) {
      return TypeErasedCallable{};
    }
    return m_interruptHandlers[static_cast<size_t>(interruptVector)];
  }

private:
  TypeErasedCallable
      m_interruptHandlers[static_cast<size_t>(InterruptVectorEnum::SIZE)] = {};
};
