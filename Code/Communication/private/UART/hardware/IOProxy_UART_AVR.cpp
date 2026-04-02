// ...existing code...
#include "Communication/UART/IOProxy_UART_AVR.h"
#include "IOProxy_UART_AVR_PeripheralMap.h"

IOProxy_UART_AVR::IOProxy_UART_AVR(uint8_t uartId) : m_uartId(uartId) {}

void IOProxy_UART_AVR::WriteRegister(UART_AVR_RegisterEnum registerName,
                                     uint8_t newValue) {
  switch (registerName) {
  case UART_AVR_RegisterEnum::REG_UDRn:
    *GetUDRAddress(m_uartId) = newValue;
    break;
  case UART_AVR_RegisterEnum::REG_UCSRnA:
    *GetUCSRnAAddress(m_uartId) = newValue;
    break;
  case UART_AVR_RegisterEnum::REG_UCSRnB:
    *GetUCSRnBAddress(m_uartId) = newValue;
    break;
  case UART_AVR_RegisterEnum::REG_UCSRnC:
    *GetUCSRnCAddress(m_uartId) = newValue;
    break;
  case UART_AVR_RegisterEnum::REG_UBRRnL:
    *GetUBRRnLAddress(m_uartId) = newValue;
    break;
  case UART_AVR_RegisterEnum::REG_UBRRnH:
    *GetUBRRnHAddress(m_uartId) = newValue;
    break;
  default:
    break;
  }
}

auto IOProxy_UART_AVR::ReadRegister(UART_AVR_RegisterEnum registerName)
    -> uint8_t {
  switch (registerName) {
  case UART_AVR_RegisterEnum::REG_UDRn:
    return *GetUDRAddress(m_uartId);
  case UART_AVR_RegisterEnum::REG_UCSRnA:
    return *GetUCSRnAAddress(m_uartId);
  case UART_AVR_RegisterEnum::REG_UCSRnB:
    return *GetUCSRnBAddress(m_uartId);
  case UART_AVR_RegisterEnum::REG_UCSRnC:
    return *GetUCSRnCAddress(m_uartId);
  case UART_AVR_RegisterEnum::REG_UBRRnL:
    return *GetUBRRnLAddress(m_uartId);
  case UART_AVR_RegisterEnum::REG_UBRRnH:
    return *GetUBRRnHAddress(m_uartId);
  default:
    return 0;
  }
}

auto IOProxy_UART_AVR::ReadDefine(UART_AVR_DefineEnum defineName) -> uint8_t {
  switch (defineName) {

    // UCSRnA
  case UART_AVR_DefineEnum::DEF_RXCn:
    return GetRXCnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_TXCn:
    return GetTXCnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_UDREn:
    return GetUDREnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_FEn:
    return GetFEnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_DORn:
    return GetDORnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_UPEn:
    return GetUPEnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_U2Xn:
    return GetU2XnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_MPCMn:
    return GetMPCMnValue(m_uartId);

  // UCSRnB
  case UART_AVR_DefineEnum::DEF_RXCIEn:
    return GetRXCIEnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_TXCIEn:
    return GetTXCIEnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_UDRIEn:
    return GetUDRIEnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_RXENn:
    return GetRXENnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_TXENn:
    return GetTXENnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_UCSZn2:
    return GetUCSZn2Value(m_uartId);
  case UART_AVR_DefineEnum::DEF_RXB8n:
    return GetRXB8nValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_TXB8n:
    return GetTXB8nValue(m_uartId);

  // UCSRnC
  case UART_AVR_DefineEnum::DEF_UMSELn1:
    return GetUMSELn1Value(m_uartId);
  case UART_AVR_DefineEnum::DEF_UMSELn0:
    return GetUMSELn0Value(m_uartId);
  case UART_AVR_DefineEnum::DEF_UPMn1:
    return GetUPMn1Value(m_uartId);
  case UART_AVR_DefineEnum::DEF_UPMn0:
    return GetUPMn0Value(m_uartId);
  case UART_AVR_DefineEnum::DEF_USBSn:
    return GetUSBSnValue(m_uartId);
  case UART_AVR_DefineEnum::DEF_UCSZn1:
    return GetUCSZn1Value(m_uartId);
  case UART_AVR_DefineEnum::DEF_UCSZn0:
    return GetUCSZn0Value(m_uartId);
  case UART_AVR_DefineEnum::DEF_UCPOLn:
    return GetUCPOLnValue(m_uartId);

  default:
    return 0;
  }
}