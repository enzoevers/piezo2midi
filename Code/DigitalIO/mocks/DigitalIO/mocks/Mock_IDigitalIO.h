#pragma once

#include <gmock/gmock.h>

#include "DigitalIO/IDigitalIO.h"

class Mock_IDigitalIO : public IDigitalIO {
public:
  MOCK_METHOD(void, SetMode, (uint8_t pin, PIN_MODE mode), (override));
  MOCK_METHOD(void, SetState, (uint8_t pin, PIN_STATE state), (override));
  MOCK_METHOD(PIN_STATE, GetState, (uint8_t pin), (override));
};