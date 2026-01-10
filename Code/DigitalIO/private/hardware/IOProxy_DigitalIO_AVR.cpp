#include "DigitalIO/IOProxy_DigitalIO_AVR.h"
#include "IOProxy/IOProxy.h"
#include "IOProxy_DigitalIO_AVR_PeripheralMap.h"

IOProxy_DigitalIO_AVR::IOProxy_DigitalIO_AVR(BANK_LETTER bankLetter)
    : m_bankLetter(bankLetter) {}

void IOProxy_DigitalIO_AVR::WriteRegister(
    DigitalIO_AVR_RegisterEnum registerName, uint8_t newValue) {
  switch (registerName) {
  case DigitalIO_AVR_RegisterEnum::REG_DDRx:
    *GetDDRAddress(m_bankLetter) = newValue;
    break;
  case DigitalIO_AVR_RegisterEnum::REG_PORTx:
    *GetPORTAddress(m_bankLetter) = newValue;
    break;
  case DigitalIO_AVR_RegisterEnum::REG_PINx:
    *GetPINAddress(m_bankLetter) = newValue;
    break;
  default:
    break;
  }
}

auto IOProxy_DigitalIO_AVR::ReadRegister(
    DigitalIO_AVR_RegisterEnum registerName) -> uint8_t {
  switch (registerName) {
  case DigitalIO_AVR_RegisterEnum::REG_DDRx:
    return *GetDDRAddress(m_bankLetter);
  case DigitalIO_AVR_RegisterEnum::REG_PORTx:
    return *GetPORTAddress(m_bankLetter);
  case DigitalIO_AVR_RegisterEnum::REG_PINx:
    return *GetPINAddress(m_bankLetter);
  default:
    return 0;
  }
}

auto IOProxy_DigitalIO_AVR::ReadDefine(
    [[maybe_unused]] DigitialIO_AVR_DefineEnum defineName) -> void {
  // No defines to read for now, but this function is here for future use and to
  // satisfy the interface requirements.
}