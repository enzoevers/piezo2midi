#pragma once

#include "Communication/UART/IIOProxy_UART_AVR.h"
#include "Communication/UART/IUART.h"
#include "Communication/UART/UART_FrameFormat.h"
#include "Communication/UART/UART_Mode.h"

// This class uses the internal clock and in in asynchronous mode.
class UART_AVR : public IUART {
public:
  UART_AVR(IIOProxy_UART_AVR &ioProxy, uint32_t baud = 9600,
           UART_FrameFormat frameFormat = {UART_DataBits::DB8,
                                           UART_Parity::NONE,
                                           UART_StopBits::SB1},
           UART_Mode mode = UART_Mode::ASYNCHRONOUS);
  ~UART_AVR() = default;

  //=== IUART implementation ===//
  void SendByte(const uint8_t byteToSend) override;
  auto ByteAvailable() -> bool override;
  auto ReadByte_NonBlocking() -> uint8_t override;
  //=== End of IUART implementation ===//

  void SetBaudRate(uint32_t baud);
  void SetFrameFormat(UART_FrameFormat frameFormat);
  void SetUARTMode(UART_Mode mode);
  void Enable();
  void Disable();

  auto GetBaudRate() const -> uint32_t;
  auto GetFrameFormat() const -> UART_FrameFormat;
  auto GetUARTMode() const -> UART_Mode;

private:
  IIOProxy_UART_AVR &m_ioProxy;
  uint32_t m_baud;
  UART_FrameFormat m_frameFormat;
  UART_Mode m_mode;
};