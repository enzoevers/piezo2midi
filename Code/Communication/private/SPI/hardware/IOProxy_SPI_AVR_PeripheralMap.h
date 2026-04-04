
#pragma once
#include <avr/io.h>
#include <stdint.h>

static auto GetSPCRAddress(uint8_t spiId) -> volatile uint8_t * {
  switch (spiId) {
#ifdef SPCR
  case 0:
    return &SPCR;
#endif
#ifdef SPCR1
  case 1:
    return &SPCR1;
#endif
  default:
    return nullptr;
  }
}

static auto GetSPSRAddress(uint8_t spiId) -> volatile uint8_t * {
  switch (spiId) {
#ifdef SPSR
  case 0:
    return &SPSR;
#endif
#ifdef SPSR1
  case 1:
    return &SPSR1;
#endif
  default:
    return nullptr;
  }
}

static auto GetSPDRAddress(uint8_t spiId) -> volatile uint8_t * {
  switch (spiId) {
#ifdef SPDR
  case 0:
    return &SPDR;
#endif
// Checking SPSR1 instead of SPDR1 since some AVR model's header files use of to
// indicate the address of the second SPI peripheral, while others use if to
// indicate the second bit in the SPDR register.
#ifdef SPSR1
  case 1:
    return &SPDR1;
#endif
  default:
    return nullptr;
  }
}

static auto GetSPIEValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef SPIE
  case 0:
    return SPIE;
#endif
#ifdef SPIE1
  case 1:
    return SPIE1;
#endif
  default:
    return 0;
  }
}

static auto GetSPEValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef SPE
  case 0:
    return SPE;
#endif
#ifdef SPE1
  case 1:
    return SPE1;
#endif
  default:
    return 0;
  }
}

static auto GetDORDValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef DORD
  case 0:
    return DORD;
#endif
#ifdef DORD1
  case 1:
    return DORD1;
#endif
  default:
    return 0;
  }
}

static auto GetMSTRValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef MSTR
  case 0:
    return MSTR;
#endif
#ifdef MSTR1
  case 1:
    return MSTR1;
#endif
  default:
    return 0;
  }
}

static auto GetCPOLValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef CPOL
  case 0:
    return CPOL;
#endif
#ifdef CPOL1
  case 1:
    return CPOL1;
#endif
  default:
    return 0;
  }
}

static auto GetCPHAValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef CPHA
  case 0:
    return CPHA;
#endif
#ifdef CPHA1
  case 1:
    return CPHA1;
#endif
  default:
    return 0;
  }
}

static auto GetSPR1Value(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef SPR1
  case 0:
    return SPR1;
#endif
#ifdef SPR11
  case 1:
    return SPR11;
#endif
  default:
    return 0;
  }
}

static auto GetSPR0Value(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef SPR0
  case 0:
    return SPR0;
#endif
#ifdef SPR01
  case 1:
    return SPR01;
#endif
  default:
    return 0;
  }
}

static auto GetSPI2XValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef SPI2X
  case 0:
    return SPI2X;
#endif
#ifdef SPI2X1
  case 1:
    return SPI2X1;
#endif
  default:
    return 0;
  }
}

static auto GetWCOLValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef WCOL
  case 0:
    return WCOL;
#endif
#ifdef WCOL1
  case 1:
    return WCOL1;
#endif
  default:
    return 0;
  }
}

static auto GetSPIFValue(uint8_t spiId) -> uint8_t {
  switch (spiId) {
#ifdef SPIF
  case 0:
    return SPIF;
#endif
#ifdef SPIF1
  case 1:
    return SPIF1;
#endif
  default:
    return 0;
  }
}
