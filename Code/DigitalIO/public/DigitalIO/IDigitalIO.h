#pragma once

#include <stdint.h>

#include "DigitalIO/BANK_LETTER.h"
#include "DigitalIO/PIN_MODE.h"
#include "DigitalIO/PIN_STATE.h"

class IDigitalIO {
public:
#ifndef __AVR__
  virtual ~IDigitalIO() = default;
#endif
  virtual void SetMode(uint8_t pin, PIN_MODE mode) = 0;
  virtual void SetState(uint8_t pin, PIN_STATE state) = 0;
  [[nodiscard]] virtual auto GetState(uint8_t pin) -> PIN_STATE = 0;
};