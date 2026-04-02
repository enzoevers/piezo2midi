#pragma once

#include "DigitalIO/BANK_LETTER.h"
#include "DigitalIO/IIOProxy_DigitalIO_AVR.h"

class IOProxy_DigitalIO_AVR : public IIOProxy_DigitalIO_AVR {
public:
  IOProxy_DigitalIO_AVR(BANK_LETTER bankLetter);
  ~IOProxy_DigitalIO_AVR() = default;

  void WriteRegister(DigitalIO_AVR_RegisterEnum registerName,
                     uint8_t newValue) override;
  auto
  ReadRegister(DigitalIO_AVR_RegisterEnum registerName) -> uint8_t override;
  auto ReadDefine(DigitialIO_AVR_DefineEnum defineName) -> void override;

private:
  const BANK_LETTER m_bankLetter;
};