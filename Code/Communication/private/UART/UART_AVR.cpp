#include "Communication/UART/UART_AVR.h"

UART_AVR::UART_AVR(IIOProxy_UART_AVR &ioProxy, uint32_t baud,
                   UART_FrameFormat frameFormat, UART_Mode mode)
    : m_ioProxy(ioProxy), m_baud(baud), m_frameFormat(frameFormat),
      m_mode(mode) {
  SetBaudRate(baud);
  SetFrameFormat(frameFormat);
  SetUARTMode(mode);
  Enable();
}

//=== IUART implementation ===//

void UART_AVR::SendByte(const uint8_t byteToSend) {
  const uint8_t udreMask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_UDREn);

  while (
      !(m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA) & udreMask)) {
    // Wait for the transmit buffer to be ready
  }
  m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UDRn,
                          static_cast<uint8_t>(byteToSend));
}

auto UART_AVR::ByteAvailable() -> bool {
  const uint8_t rxcMask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_RXCn);
  return (m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA) & rxcMask);
}

auto UART_AVR::ReadByte_NonBlocking() -> uint8_t {
  if (ByteAvailable()) {
    return m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UDRn);
  }
  return 0;
}

//=== IUART implementation ===//

void UART_AVR::SetBaudRate(uint32_t baud) {
  // AVR has a prescale of 16 for asynchronous normal mode. This can be reduced
  // to 8 if the U2Xn bit in UCSRnA is set. First the default prescale of 16 is
  // used to calculate the UBRR value. If the resulting baud rate is outside the
  // tolerance, then the U2Xn bit is set and the UBRR value is recalculated with
  // a prescale of 8.

  static constexpr uint32_t tolerance = 1; // 1% tolerance
  static constexpr uint32_t f_cpu = F_CPU;

  // Calculate UBRR value for prescale of 16
  uint32_t ubrrValue = (f_cpu / (16UL * baud)) - 1;

  const uint32_t actualBaud = f_cpu / (16UL * (ubrrValue + 1));

  const uint32_t baudError =
      (actualBaud > baud) ? (actualBaud - baud) : (baud - actualBaud);
  const uint32_t baudErrorPercent = (baudError * 100) / baud;

  if (baudErrorPercent > tolerance) {
    // Set U2Xn bit for prescale of 8
    const uint8_t newCSRnAValue =
        m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA) |
        (1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_U2Xn));
    m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnA, newCSRnAValue);

    // Recalculate UBRR value for prescale of 8
    ubrrValue = (f_cpu / (8UL * baud)) - 1;
  } else {
    // Clear U2Xn bit for prescale of 16
    const uint8_t newCSRnAValue =
        m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA) &
        ~(1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_U2Xn));
    m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnA, newCSRnAValue);
  }

  // Set UBRR registers
  m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UBRRnH,
                          (ubrrValue >> 8) & 0xFF);
  m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UBRRnL, ubrrValue & 0xFF);

  // Set the member variable after successfully setting the baud rate
  m_baud = baud;
}

void UART_AVR::SetFrameFormat(UART_FrameFormat frameFormat) {
  uint8_t UCSRnBValue =
      m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnB);
  uint8_t UCSRnCValue =
      m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnC);

  const uint8_t ucsmzn2Mask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_UCSZn2);
  const uint8_t ucsmzn1Mask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_UCSZn1);
  const uint8_t ucsmzn0Mask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_UCSZn0);

  const uint8_t upmn1Mask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_UPMn1);
  const uint8_t upmn0Mask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_UPMn0);

  const uint8_t usbsnMask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_USBSn);

  switch (frameFormat.dataBits) {
  case UART_DataBits::DB5:
    UCSRnBValue &= ~ucsmzn2Mask;
    UCSRnCValue &= ~ucsmzn1Mask;
    UCSRnCValue &= ~ucsmzn0Mask;
    break;
  case UART_DataBits::DB6:
    UCSRnBValue &= ~ucsmzn2Mask;
    UCSRnCValue &= ~ucsmzn1Mask;
    UCSRnCValue |= ucsmzn0Mask;
    break;
  case UART_DataBits::DB7:
    UCSRnBValue &= ~ucsmzn2Mask;
    UCSRnCValue |= ucsmzn1Mask;
    UCSRnCValue &= ~ucsmzn0Mask;
    break;
  case UART_DataBits::DB8:
    UCSRnBValue &= ~ucsmzn2Mask;
    UCSRnCValue |= ucsmzn1Mask;
    UCSRnCValue |= ucsmzn0Mask;
    break;
  // DB9 is not supported for now
  default:
    break;
  };

  switch (frameFormat.parity) {
  case UART_Parity::NONE:
    UCSRnCValue &= ~upmn1Mask;
    UCSRnCValue &= ~upmn0Mask;
    break;
  case UART_Parity::EVEN:
    UCSRnCValue |= upmn1Mask;
    UCSRnCValue &= ~upmn0Mask;
    break;
  case UART_Parity::ODD:
    UCSRnCValue |= upmn1Mask;
    UCSRnCValue |= upmn0Mask;
    break;
  default:
    break;
  };

  switch (frameFormat.stopBits) {
  case UART_StopBits::SB1:
    UCSRnCValue &= ~usbsnMask;
    break;
  case UART_StopBits::SB2:
    UCSRnCValue |= usbsnMask;
    break;
  default:
    break;
  };

  m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB, UCSRnBValue);
  m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnC, UCSRnCValue);

  m_frameFormat = frameFormat;
}

void UART_AVR::SetUARTMode(UART_Mode mode) {
  uint8_t UCSRnCValue =
      m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnC);
  const uint8_t umseln1Mask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_UMSELn1);
  const uint8_t umseln0Mask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_UMSELn0);

  switch (mode) {
  case UART_Mode::ASYNCHRONOUS:
    UCSRnCValue &= ~umseln1Mask;
    UCSRnCValue &= ~umseln0Mask;
    break;
  case UART_Mode::SYNCHRONOUS:
    UCSRnCValue &= ~umseln1Mask;
    UCSRnCValue |= umseln0Mask;
    break;
  default:
    break;
  };

  m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnC, UCSRnCValue);

  m_mode = mode;
}

void UART_AVR::Enable() {
  uint8_t ucsrnbValue =
      m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnB);
  const uint8_t rxenMask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_RXENn);
  const uint8_t txenMask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_TXENn);

  ucsrnbValue |= rxenMask;
  ucsrnbValue |= txenMask;

  m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB, ucsrnbValue);
}

void UART_AVR::Disable() {
  uint8_t ucsrnbValue =
      m_ioProxy.ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnB);
  const uint8_t rxenMask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_RXENn);
  const uint8_t txenMask =
      1 << m_ioProxy.ReadDefine(UART_AVR_DefineEnum::DEF_TXENn);

  ucsrnbValue &= ~rxenMask;
  ucsrnbValue &= ~txenMask;

  m_ioProxy.WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB, ucsrnbValue);
}

auto UART_AVR::GetBaudRate() const -> uint32_t { return m_baud; }

auto UART_AVR::GetFrameFormat() const -> UART_FrameFormat {
  return m_frameFormat;
}

auto UART_AVR::GetUARTMode() const -> UART_Mode { return m_mode; }
