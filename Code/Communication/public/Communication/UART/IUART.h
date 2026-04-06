#pragma once

#include <stdint.h>

class IUART {
public:
#ifndef __AVR__
  virtual ~IUART() = default;
#endif

  virtual void SendByte(const uint8_t byteToSend) = 0;
  virtual auto ByteAvailable() -> bool = 0;
  virtual auto ReadByte_NonBlocking() -> uint8_t = 0;
};