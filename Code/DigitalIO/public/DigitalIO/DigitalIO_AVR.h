#pragma once

#include "DigitalIO/IDigitalIO.h"
#include "DigitalIO/IIOProxy_DigitalIO_AVR.h"

class DigitalIO_AVR : public IDigitalIO {
public:
  DigitalIO_AVR(IIOProxy_DigitalIO_AVR &ioProxy);
  ~DigitalIO_AVR() = default;

  void SetMode(uint8_t pin, PIN_MODE mode) override;
  void SetState(uint8_t pin, PIN_STATE state) override;
  [[nodiscard]] auto GetState(uint8_t pin) -> PIN_STATE override;

private:
  IIOProxy_DigitalIO_AVR &m_ioProxy;
};