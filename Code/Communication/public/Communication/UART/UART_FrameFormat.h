#pragma once

#include <stdint.h>

enum class UART_DataBits : uint8_t {
  DB5 = 5,
  DB6 = 6,
  DB7 = 7,
  DB8 = 8,
  // DB9 = 9 // This library won't support 9 bits for now.
};

enum class UART_Parity : uint8_t { NONE, EVEN, ODD };

enum class UART_StopBits : uint8_t { SB1 = 1, SB2 = 2 };

struct UART_FrameFormat {
  UART_DataBits dataBits;
  UART_Parity parity;
  UART_StopBits stopBits;

  bool operator==(const UART_FrameFormat &other) const {
    return dataBits == other.dataBits && parity == other.parity &&
           stopBits == other.stopBits;
  }
};