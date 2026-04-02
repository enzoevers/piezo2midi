#pragma once

#include <gmock/gmock.h>

#include "DigitalIO/IDigitalIO.h"

class Mock_IDigitalIO : public IDigitalIO {
public:
  MOCK_METHOD(void, SetMode, (PinName pinName, PIN_MODE mode), (override));
  MOCK_METHOD(void, SetState, (PinName pinName, PIN_STATE state), (override));
  MOCK_METHOD(PIN_STATE, GetState, (PinName pinName), (override));
};