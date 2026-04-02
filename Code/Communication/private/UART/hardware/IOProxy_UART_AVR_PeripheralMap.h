
#pragma once
#include <avr/io.h>
#include <stdint.h>

static auto GetUDRAddress(uint8_t uartId) -> volatile uint8_t * {
  switch (uartId) {
#ifdef UDR0
  case 0:
    return &UDR0;
#endif
#ifdef UDR1
  case 1:
    return &UDR1;
#endif
  default:
    return nullptr;
  }
}

static auto GetUCSRnAAddress(uint8_t uartId) -> volatile uint8_t * {
  switch (uartId) {
#ifdef UCSR0A
  case 0:
    return &UCSR0A;
#endif
#ifdef UCSR1A
  case 1:
    return &UCSR1A;
#endif
  default:
    return nullptr;
  }
}

static auto GetUCSRnBAddress(uint8_t uartId) -> volatile uint8_t * {
  switch (uartId) {
#ifdef UCSR0B
  case 0:
    return &UCSR0B;
#endif
#ifdef UCSR1B
  case 1:
    return &UCSR1B;
#endif
  default:
    return nullptr;
  }
}

static auto GetUCSRnCAddress(uint8_t uartId) -> volatile uint8_t * {
  switch (uartId) {
#ifdef UCSR0C
  case 0:
    return &UCSR0C;
#endif
#ifdef UCSR1C
  case 1:
    return &UCSR1C;
#endif
  default:
    return nullptr;
  }
}

static auto GetUBRRnLAddress(uint8_t uartId) -> volatile uint8_t * {
  switch (uartId) {
#ifdef UBRR0L
  case 0:
    return &UBRR0L;
#endif
#ifdef UBRR1L
  case 1:
    return &UBRR1L;
#endif
  default:
    return nullptr;
  }
}

static auto GetUBRRnHAddress(uint8_t uartId) -> volatile uint8_t * {
  switch (uartId) {
#ifdef UBRR0H
  case 0:
    return &UBRR0H;
#endif
#ifdef UBRR1H
  case 1:
    return &UBRR1H;
#endif
  default:
    return nullptr;
  }
}

static auto GetRXCnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef RXC0
  case 0:
    return RXC0;
#endif
#ifdef RXC1
  case 1:
    return RXC1;
#endif
  default:
    return 0;
  }
}

static auto GetTXCnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef TXC0
  case 0:
    return TXC0;
#endif
#ifdef TXC1
  case 1:
    return TXC1;
#endif
  default:
    return 0;
  }
}

static auto GetUDREnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UDRE0
  case 0:
    return UDRE0;
#endif
#ifdef UDRE1
  case 1:
    return UDRE1;
#endif
  default:
    return 0;
  }
}

static auto GetFEnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef FE0
  case 0:
    return FE0;
#endif
#ifdef FE1
  case 1:
    return FE1;
#endif
  default:
    return 0;
  }
}

static auto GetDORnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef DOR0
  case 0:
    return DOR0;
#endif
#ifdef DOR1
  case 1:
    return DOR1;
#endif
  default:
    return 0;
  }
}

static auto GetUPEnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UPE0
  case 0:
    return UPE0;
#endif
#ifdef UPE1
  case 1:
    return UPE1;
#endif
  default:
    return 0;
  }
}

static auto GetU2XnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef U2X0
  case 0:
    return U2X0;
#endif
#ifdef U2X1
  case 1:
    return U2X1;
#endif
  default:
    return 0;
  }
}

static auto GetMPCMnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef MPCM0
  case 0:
    return MPCM0;
#endif
#ifdef MPCM1
  case 1:
    return MPCM1;
#endif
  default:
    return 0;
  }
}

static auto GetRXCIEnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef RXCIE0
  case 0:
    return RXCIE0;
#endif
#ifdef RXCIE1
  case 1:
    return RXCIE1;
#endif
  default:
    return 0;
  }
}

static auto GetTXCIEnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef TXCIE0
  case 0:
    return TXCIE0;
#endif
#ifdef TXCIE1
  case 1:
    return TXCIE1;
#endif
  default:
    return 0;
  }
}

static auto GetUDRIEnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UDRIE0
  case 0:
    return UDRIE0;
#endif
#ifdef UDRIE1
  case 1:
    return UDRIE1;
#endif
  default:
    return 0;
  }
}

static auto GetRXENnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef RXEN0
  case 0:
    return RXEN0;
#endif
#ifdef RXEN1
  case 1:
    return RXEN1;
#endif
  default:
    return 0;
  }
}

static auto GetTXENnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef TXEN0
  case 0:
    return TXEN0;
#endif
#ifdef TXEN1
  case 1:
    return TXEN1;
#endif
  default:
    return 0;
  }
}

static auto GetUCSZn2Value(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UCSZ02
  case 0:
    return UCSZ02;
#endif
#ifdef UCSZ12
  case 1:
    return UCSZ12;
#endif
  default:
    return 0;
  }
}

static auto GetRXB8nValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef RXB80
  case 0:
    return RXB80;
#endif
#ifdef RXB81
  case 1:
    return RXB81;
#endif
  default:
    return 0;
  }
}

static auto GetTXB8nValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef TXB80
  case 0:
    return TXB80;
#endif
#ifdef TXB81
  case 1:
    return TXB81;
#endif
  default:
    return 0;
  }
}

static auto GetUMSELn1Value(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UMSEL01
  case 0:
    return UMSEL01;
#endif
#ifdef UMSEL11
  case 1:
    return UMSEL11;
#endif
  default:
    return 0;
  }
}

static auto GetUMSELn0Value(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UMSEL00
  case 0:
    return UMSEL00;
#endif
#ifdef UMSEL10
  case 1:
    return UMSEL10;
#endif
  default:
    return 0;
  }
}

static auto GetUPMn0Value(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UPM00
  case 0:
    return UPM00;
#endif
#ifdef UPM10
  case 1:
    return UPM10;
#endif
  default:
    return 0;
  }
}

static auto GetUPMn1Value(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UPM01
  case 0:
    return UPM01;
#endif
#ifdef UPM11
  case 1:
    return UPM11;
#endif
  default:
    return 0;
  }
}

static auto GetUSBSnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef USBS0
  case 0:
    return USBS0;
#endif
#ifdef USBS1
  case 1:
    return USBS1;
#endif
  default:
    return 0;
  }
}

static auto GetUCSZn1Value(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UCSZ01
  case 0:
    return UCSZ01;
#endif
#ifdef UCSZ11
  case 1:
    return UCSZ11;
#endif
  default:
    return 0;
  }
}

static auto GetUCSZn0Value(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UCSZ00
  case 0:
    return UCSZ00;
#endif
#ifdef UCSZ10
  case 1:
    return UCSZ10;
#endif
  default:
    return 0;
  }
}

static auto GetUCPOLnValue(uint8_t uartId) -> uint8_t {
  switch (uartId) {
#ifdef UCPOL0
  case 0:
    return UCPOL0;
#endif
#ifdef UCPOL1
  case 1:
    return UCPOL1;
#endif
  default:
    return 0;
  }
}