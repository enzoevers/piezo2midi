#include "DigitalIO/DigitalIO_AVR.h"
#include "DigitalIO/IOProxy_DigitalIO_AVR.h"

DigitalIO_AVR::DigitalIO_AVR(IIOProxy_DigitalIO_AVR &ioProxy)
    : m_ioProxy(ioProxy) {}

void DigitalIO_AVR::SetMode(uint8_t pin, PIN_MODE mode) {
  const auto currentValue =
      m_ioProxy.ReadRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx);

  switch (mode) {
  case PIN_MODE::INPUT:
    m_ioProxy.WriteRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx,
                            currentValue & ~(1 << pin));
    break;
  case PIN_MODE::OUTPUT:
    m_ioProxy.WriteRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx,
                            currentValue | (1 << pin));
    break;
  }
}

void DigitalIO_AVR::SetState(uint8_t pin, PIN_STATE state) {
  const auto currentValue =
      m_ioProxy.ReadRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx);

  switch (state) {
  case PIN_STATE::HIGH:
    m_ioProxy.WriteRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx,
                            currentValue | (1 << pin));
    break;
  case PIN_STATE::LOW:
    m_ioProxy.WriteRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx,
                            currentValue & ~(1 << pin));
    break;
  }
}

auto DigitalIO_AVR::GetState(uint8_t pin) -> PIN_STATE {
  const auto currentValue =
      m_ioProxy.ReadRegister(DigitalIO_AVR_RegisterEnum::REG_PINx);
  return (currentValue & (1 << pin)) ? PIN_STATE::HIGH : PIN_STATE::LOW;
}