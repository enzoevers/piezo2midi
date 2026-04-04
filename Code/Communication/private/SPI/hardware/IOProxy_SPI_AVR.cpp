// ...existing code...
#include "Communication/SPI/IOProxy_SPI_AVR.h"
#include "IOProxy_SPI_AVR_PeripheralMap.h"

IOProxy_SPI_AVR::IOProxy_SPI_AVR(uint8_t spiId) : m_spiId(spiId) {}

void IOProxy_SPI_AVR::WriteRegister(SPI_AVR_RegisterEnum registerName,
                                    uint8_t newValue) {
  switch (registerName) {
  case SPI_AVR_RegisterEnum::REG_SPCRx:
    *GetSPCRAddress(m_spiId) = newValue;
    break;
  case SPI_AVR_RegisterEnum::REG_SPSRx:
    *GetSPSRAddress(m_spiId) = newValue;
    break;
  case SPI_AVR_RegisterEnum::REG_SPDRx:
    *GetSPDRAddress(m_spiId) = newValue;
    break;
  default:
    break;
  }
}

auto IOProxy_SPI_AVR::ReadRegister(SPI_AVR_RegisterEnum registerName)
    -> uint8_t {
  switch (registerName) {
  case SPI_AVR_RegisterEnum::REG_SPCRx:
    return *GetSPCRAddress(m_spiId);
  case SPI_AVR_RegisterEnum::REG_SPSRx:
    return *GetSPSRAddress(m_spiId);
  case SPI_AVR_RegisterEnum::REG_SPDRx:
    return *GetSPDRAddress(m_spiId);
  default:
    return 0;
  }
}

auto IOProxy_SPI_AVR::ReadDefine(SPI_AVR_DefineEnum defineName) -> uint8_t {
  switch (defineName) {

    // SPCRx
  case SPI_AVR_DefineEnum::DEF_SPIE:
    return GetSPIEValue(m_spiId);
  case SPI_AVR_DefineEnum::DEF_SPE:
    return GetSPEValue(m_spiId);
  case SPI_AVR_DefineEnum::DEF_DORD:
    return GetDORDValue(m_spiId);
  case SPI_AVR_DefineEnum::DEF_MSTR:
    return GetMSTRValue(m_spiId);
  case SPI_AVR_DefineEnum::DEF_CPOL:
    return GetCPOLValue(m_spiId);
  case SPI_AVR_DefineEnum::DEF_CPHA:
    return GetCPHAValue(m_spiId);
  case SPI_AVR_DefineEnum::DEF_SPR1:
    return GetSPR1Value(m_spiId);
  case SPI_AVR_DefineEnum::DEF_SPR0:
    return GetSPR0Value(m_spiId);

    // SPSRx
  case SPI_AVR_DefineEnum::DEF_SPIF:
    return GetSPIFValue(m_spiId);
  case SPI_AVR_DefineEnum::DEF_WCOL:
    return GetWCOLValue(m_spiId);
  case SPI_AVR_DefineEnum::DEF_SPI2X:
    return GetSPI2XValue(m_spiId);

  default:
    return 0;
  }
}