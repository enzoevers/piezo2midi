#include "Communication/SPI/SPIMaster_AVR.h"

SPIMaster_AVR::SPIMaster_AVR(IIOProxy_SPI_AVR &ioProxy, SPIPinConfig pinConfig,
                             SPI_MODE mode, uint32_t clk_hz,
                             SPI_DATAORDER dataOrder)
    : m_ioProxy(ioProxy), m_pinConfig(pinConfig) {
  SetPinModes();
  SetMasterMode();
  SetMode(mode);
  SetClockFrequency(clk_hz);
  SetDataOrder(dataOrder);
  EnableSPI();
}

//=== ISPIMaster implementation ===//

auto SPIMaster_AVR::SelectSlave(uint8_t slaveIndex) -> bool {
  if (slaveIndex >= m_pinConfig.slaveCount) {
    return false;
  }

  const auto &selectedSlavePin = m_pinConfig.SS_pins[slaveIndex];
  selectedSlavePin.digitalIO.SetState(selectedSlavePin.pin, PIN_STATE::LOW);

  return true;
}

auto SPIMaster_AVR::DeselectSlave(uint8_t slaveIndex) -> bool {
  if (slaveIndex >= m_pinConfig.slaveCount) {
    return false;
  }

  const auto &selectedSlavePin = m_pinConfig.SS_pins[slaveIndex];
  selectedSlavePin.digitalIO.SetState(selectedSlavePin.pin, PIN_STATE::HIGH);

  return true;
}

auto SPIMaster_AVR::SendByte(const uint8_t byteToSend) -> uint8_t {
  m_ioProxy.WriteRegister(SPI_AVR_RegisterEnum::REG_SPDRx, byteToSend);

  const uint8_t spifMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPIF);
  while (
      !(m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx) & spifMask)) {
    // Wait for transmission to complete
  }

  const uint8_t receivedByte =
      m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPDRx);
  return receivedByte;
}

//=== End of ISPIMaster implementation ===//

void SPIMaster_AVR::SetMode(SPI_MODE mode) {
  const uint8_t cpolMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_CPOL);
  const uint8_t cphaMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_CPHA);

  uint8_t spcrxValue = m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);

  switch (mode) {
  case SPI_MODE::MODE0:
    spcrxValue &= ~cpolMask;
    spcrxValue &= ~cphaMask;
    break;
  case SPI_MODE::MODE1:
    spcrxValue &= ~cpolMask;
    spcrxValue |= cphaMask;
    break;
  case SPI_MODE::MODE2:
    spcrxValue |= cpolMask;
    spcrxValue &= ~cphaMask;
    break;
  case SPI_MODE::MODE3:
    spcrxValue |= cpolMask;
    spcrxValue |= cphaMask;
    break;
  default:
    break;
  }

  m_ioProxy.WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx, spcrxValue);
}

void SPIMaster_AVR::SetClockFrequency(uint32_t clk_hz,
                                      ROUND_DIRECTION roundDirection) {
  const uint32_t f_cpu = F_CPU;
  const uint32_t clkDiv = f_cpu / clk_hz;
  const uint32_t remainder = f_cpu - (clkDiv * clk_hz);

  uint8_t spcrxValue = m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);
  uint8_t spsrxValue = m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx);
  const uint8_t spr1Mask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPR1);
  const uint8_t spr0Mask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPR0);
  const uint8_t spi2xMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPI2X);

  int8_t largestOneIndex = -1;
  uint32_t tempClkDiv = clkDiv;
  while (tempClkDiv > 0) {
    tempClkDiv >>= 1;
    ++largestOneIndex;
  }

  uint8_t closestDivider = 2; // Default to the smallest divider

  if (largestOneIndex <= 0) {
    closestDivider = 2;
  } else if (largestOneIndex >= 8) {
    closestDivider = 128;
  } else if (remainder == 0) {
    closestDivider = static_cast<uint8_t>(clkDiv);
  } else {
    switch (roundDirection) {
    case ROUND_DIRECTION::UP:
      closestDivider = 1 << largestOneIndex;
      break;
    case ROUND_DIRECTION::DOWN:
      closestDivider = 1 << (largestOneIndex + 1);
      break;
    case ROUND_DIRECTION::CLOSEST:
      if (clkDiv & (1 << (largestOneIndex - 1))) {
        closestDivider = 1 << (largestOneIndex + 1);
      } else {
        closestDivider = 1 << largestOneIndex;
      }
      break;
    }
  }

  switch (closestDivider) {
  case 2:
    spsrxValue |= spi2xMask;
    spcrxValue &= ~spr1Mask;
    spcrxValue &= ~spr0Mask;
    break;
  case 4:
    spsrxValue &= ~spi2xMask;
    spcrxValue &= ~spr1Mask;
    spcrxValue &= ~spr0Mask;
    break;
  case 8:
    spsrxValue |= spi2xMask;
    spcrxValue &= ~spr1Mask;
    spcrxValue |= spr0Mask;
    break;
  case 16:
    spsrxValue &= ~spi2xMask;
    spcrxValue &= ~spr1Mask;
    spcrxValue |= spr0Mask;
    break;
  case 32:
    spsrxValue |= spi2xMask;
    spcrxValue |= spr1Mask;
    spcrxValue &= ~spr0Mask;
    break;
  case 64:
    spsrxValue &= ~spi2xMask;
    spcrxValue |= spr1Mask;
    spcrxValue &= ~spr0Mask;
    break;
  case 128:
    spsrxValue &= ~spi2xMask;
    spcrxValue |= spr1Mask;
    spcrxValue |= spr0Mask;
    break;

  default:
    break; // Should not happen
  }

  m_ioProxy.WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx, spcrxValue);
  m_ioProxy.WriteRegister(SPI_AVR_RegisterEnum::REG_SPSRx, spsrxValue);
}

void SPIMaster_AVR::SetDataOrder(SPI_DATAORDER dataOrder) {
  uint8_t spcrxValue = m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);
  const uint8_t dordMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_DORD);

  switch (dataOrder) {
  case SPI_DATAORDER::MSB_FIRST:
    spcrxValue &= ~dordMask;
    break;
  case SPI_DATAORDER::LSB_FIRST:
    spcrxValue |= dordMask;
    break;
  default:
    break;
  }

  m_ioProxy.WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx, spcrxValue);
}

auto SPIMaster_AVR::GetMode() const -> SPI_MODE {
  uint8_t spcrxValue = m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);
  const uint8_t cpolMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_CPOL);
  const uint8_t cphaMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_CPHA);

  const uint8_t cpolSet = (spcrxValue & cpolMask) != 0;
  const uint8_t cphaSet = (spcrxValue & cphaMask) != 0;

  const uint8_t modeId = (cpolSet << 1) | cphaSet;

  switch (modeId) {
  case 0b0000'0000:
    return SPI_MODE::MODE0;
  case 0b0000'0001:
    return SPI_MODE::MODE1;
  case 0b0000'0010:
    return SPI_MODE::MODE2;
  case 0b0000'0011:
    return SPI_MODE::MODE3;
  default:
    // Should not happen, but return a default value just in case
    return SPI_MODE::MODE0;
  }
}

auto SPIMaster_AVR::GetClockFrequency() const -> uint32_t {
  const uint8_t spcrxValue =
      m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);
  const uint8_t spsrxValue =
      m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx);
  const uint8_t spr1Mask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPR1);
  const uint8_t spr0Mask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPR0);
  const uint8_t spi2xMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPI2X);

  const uint8_t spi2xSet = (spsrxValue & spi2xMask) != 0;
  const uint8_t spr1Set = (spcrxValue & spr1Mask) != 0;
  const uint8_t spr0Set = (spcrxValue & spr0Mask) != 0;

  const uint8_t dividerId = (spi2xSet << 2) | (spr1Set << 1) | spr0Set;

  const uint32_t f_cpu = F_CPU;
  uint32_t clkDiv = 1;
  switch (dividerId) {
  case 0b0000'0000:
    clkDiv = 4;
    break;
  case 0b0000'0001:
    clkDiv = 16;
    break;
  case 0b0000'0010:
    clkDiv = 64;
    break;
  case 0b0000'0011:
    clkDiv = 128;
    break;
  case 0b0000'0100:
    clkDiv = 2;
    break;
  case 0b0000'0101:
    clkDiv = 8;
    break;
  case 0b0000'0110:
    clkDiv = 32;
    break;
  case 0b0000'0111:
    clkDiv = 64;
    break;
  default:
    // Should not happen
    clkDiv = 1;
    break;
  }

  return f_cpu / clkDiv;
}

auto SPIMaster_AVR::GetDataOrder() const -> SPI_DATAORDER {
  const uint8_t spcrxValue =
      m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);
  const uint8_t dordMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_DORD);
  const bool dordSet = (spcrxValue & dordMask) != 0;

  return dordSet ? SPI_DATAORDER::LSB_FIRST : SPI_DATAORDER::MSB_FIRST;
}

void SPIMaster_AVR::SetPinModes() {
  m_pinConfig.MOSI_pin.digitalIO.SetMode(m_pinConfig.MOSI_pin.pin,
                                         PIN_MODE::OUTPUT);
  // MISO direction is set by the hardware, so we don't set it here
  m_pinConfig.SCLK_pin.digitalIO.SetMode(m_pinConfig.SCLK_pin.pin,
                                         PIN_MODE::OUTPUT);
  for (uint8_t i = 0; i < m_pinConfig.slaveCount; ++i) {
    m_pinConfig.SS_pins[i].digitalIO.SetMode(m_pinConfig.SS_pins[i].pin,
                                             PIN_MODE::OUTPUT);
    m_pinConfig.SS_pins[i].digitalIO.SetState(m_pinConfig.SS_pins[i].pin,
                                              PIN_STATE::HIGH);
  }
}

void SPIMaster_AVR::SetMasterMode() {
  uint8_t spcrxValue = m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);
  const uint8_t mstrMask =
      1 << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_MSTR);

  spcrxValue |= mstrMask;

  m_ioProxy.WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx, spcrxValue);
}

void SPIMaster_AVR::EnableSPI() {
  uint8_t spcrxValue = m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);
  const uint8_t speMask = 1
                          << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPE);

  spcrxValue |= speMask;

  m_ioProxy.WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx, spcrxValue);
}

void SPIMaster_AVR::DisableSPI() {
  uint8_t spcrxValue = m_ioProxy.ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx);
  const uint8_t speMask = 1
                          << m_ioProxy.ReadDefine(SPI_AVR_DefineEnum::DEF_SPE);

  spcrxValue &= ~speMask;

  m_ioProxy.WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx, spcrxValue);
}
