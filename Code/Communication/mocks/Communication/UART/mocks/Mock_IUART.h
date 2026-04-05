#pragma once

#include "Communication/UART/IUART.h"
#include <gmock/gmock.h>
#include <stdint.h>

class Mock_IUART : public IUART {
public:
  ~Mock_IUART() override = default;

  MOCK_METHOD(void, SendByte, (const uint8_t byte), (override));
  MOCK_METHOD(bool, ByteAvailable, (), (override));
  MOCK_METHOD(uint8_t, ReadByte_NonBlocking, (), (override));
};