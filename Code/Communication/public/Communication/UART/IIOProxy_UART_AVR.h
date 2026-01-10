#pragma once

#include "IOProxy/IOProxy.h"
#include <stdint.h>

enum class UART_AVR_RegisterEnum {
  REG_UDRn,
  REG_UCSRnA,
  REG_UCSRnB,
  REG_UCSRnC,
  REG_UBRRnL,
  REG_UBRRnH,

  INVALID
};

enum class UART_AVR_DefineEnum {
  // UCSRnA
  DEF_RXCn,
  DEF_TXCn,
  DEF_UDREn,
  DEF_FEn,
  DEF_DORn,
  DEF_UPEn,
  DEF_U2Xn,
  DEF_MPCMn,

  // UCSRnB
  DEF_RXCIEn,
  DEF_TXCIEn,
  DEF_UDRIEn,
  DEF_RXENn,
  DEF_TXENn,
  DEF_UCSZn2,
  DEF_RXB8n,
  DEF_TXB8n,

  // UCSRnC
  DEF_UMSELn1,
  DEF_UMSELn0,
  DEF_UPMn1,
  DEF_UPMn0,
  DEF_USBSn,
  DEF_UCSZn1,
  DEF_UCSZn0,
  DEF_UCPOLn,

  INVALID
};

class IIOProxy_UART_AVR : public IOProxy<UART_AVR_RegisterEnum, uint8_t,
                                         UART_AVR_DefineEnum, uint8_t> {
public:
#ifndef __AVR__
  virtual ~IIOProxy_UART_AVR() = default;
#endif
  virtual void WriteRegister(UART_AVR_RegisterEnum registerName,
                             uint8_t newValue) = 0;
  virtual auto ReadRegister(UART_AVR_RegisterEnum registerName) -> uint8_t = 0;
  virtual auto ReadDefine(UART_AVR_DefineEnum defineName) -> uint8_t = 0;
};
