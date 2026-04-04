#pragma once

#include "Communication/SPI/ISPIMaster.h"
#include <gmock/gmock.h>

class Mock_ISPIMaster : public ISPIMaster {
public:
  ~Mock_ISPIMaster() override = default;

  MOCK_METHOD(bool, SelectSlave, (uint8_t slaveIndex), (override));
  MOCK_METHOD(bool, DeselectSlave, (uint8_t slaveIndex), (override));
  MOCK_METHOD(uint8_t, SendByte, (const uint8_t byteToSend), (override));
};
