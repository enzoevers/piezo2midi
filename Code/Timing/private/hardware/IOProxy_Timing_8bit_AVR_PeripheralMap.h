#pragma once

#include <avr/io.h>

static auto GetTCCRAddress(uint8_t timerId) -> volatile uint8_t * {
  switch (timerId) {
#ifdef TCCR0A
  case 0:
    return &TCCR0A;
#endif
#ifdef TCCR1A
  case 1:
    return &TCCR1A;
#endif
  default:
    return nullptr; // Invalid timer ID
  }
}

static auto GetTCCRBAddress(uint8_t timerId) -> volatile uint8_t * {
  switch (timerId) {
#ifdef TCCR0B
  case 0:
    return &TCCR0B;
#endif
#ifdef TCCR1B
  case 1:
    return &TCCR1B;
#endif
  default:
    return nullptr; // Invalid timer ID
  }
}

static auto GetTCNTAddress(uint8_t timerId) -> volatile uint8_t * {
  switch (timerId) {
#if defined(TCNT0) && !defined(TCNT0L) && !defined(TCNT0H)
  case 0:
    return &TCNT0;
#endif
#if defined(TCNT1) && !defined(TCNT1L) && !defined(TCNT1H)
  case 1:
    return &TCNT1;
#endif
  default:
    return nullptr; // Invalid timer ID
  }
}

static auto GetOCRAAddress(uint8_t timerId) -> volatile uint8_t * {
  switch (timerId) {
#if defined(OCR0A) && !defined(OCR0AL) && !defined(OCR0AH)
  case 0:
    return &OCR0A;
#endif
#if defined(OCR1A) && !defined(OCR1AL) && !defined(OCR1AH)
  case 1:
    return &OCR1A;
#endif
  default:
    return nullptr; // Invalid timer ID
  }
}

static auto GetOCRBAddress(uint8_t timerId) -> volatile uint8_t * {
  switch (timerId) {
#if defined(OCR0B) && !defined(OCR0BL) && !defined(OCR0BH)
  case 0:
    return &OCR0B;
#endif
#if defined(OCR1B) && !defined(OCR1BL) && !defined(OCR1BH)
  case 1:
    return &OCR1B;
#endif
  default:
    return nullptr; // Invalid timer ID
  }
}

static auto GetTIMSKAddress(uint8_t timerId) -> volatile uint8_t * {
  switch (timerId) {
#ifdef TIMSK0
  case 0:
    return &TIMSK0;
#endif
#ifdef TIMSK1
  case 1:
    return &TIMSK1;
#endif
  default:
    return nullptr; // Invalid timer ID
  }
}

static auto GetTIFRAddress(uint8_t timerId) -> volatile uint8_t * {
  switch (timerId) {
#ifdef TIFR0
  case 0:
    return &TIFR0;
#endif
#ifdef TIFR1
  case 1:
    return &TIFR1;
#endif
  default:
    return nullptr; // Invalid timer ID
  }
}

static auto GetCOMA1Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef COM0A1
  case 0:
    return COM0A1;
#endif
#ifdef COM1A1
  case 1:
    return COM1A1;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetCOMA0Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef COM0A0
  case 0:
    return COM0A0;
#endif
#ifdef COM1A0
  case 1:
    return COM1A0;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetCOMB1Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef COM0B1
  case 0:
    return COM0B1;
#endif
#ifdef COM1B1
  case 1:
    return COM1B1;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetCOMB0Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef COM0B0
  case 0:
    return COM0B0;
#endif
#ifdef COM1B0
  case 1:
    return COM1B0;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetWGM1Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef WGM01
  case 0:
    return WGM01;
#endif
#ifdef WGM11
  case 1:
    return WGM11;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetWGM0Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef WGM00
  case 0:
    return WGM00;
#endif
#ifdef WGM10
  case 1:
    return WGM10;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetFOCAValue(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef FOC0A
  case 0:
    return FOC0A;
#endif
#ifdef FOC1A
  case 1:
    return FOC1A;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetFOCBValue(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef FOC0B
  case 0:
    return FOC0B;
#endif
#ifdef FOC1B
  case 1:
    return FOC1B;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetWGM2Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef WGM02
  case 0:
    return WGM02;
#endif
#ifdef WGM12
  case 1:
    return WGM12;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetCS2Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef CS02
  case 0:
    return CS02;
#endif
#ifdef CS12
  case 1:
    return CS12;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetCS1Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef CS01
  case 0:
    return CS01;
#endif
#ifdef CS11
  case 1:
    return CS11;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetCS0Value(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef CS00
  case 0:
    return CS00;
#endif
#ifdef CS10
  case 1:
    return CS10;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetOCIEBValue(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef OCIE0B
  case 0:
    return OCIE0B;
#endif
#ifdef OCIE1B
  case 1:
    return OCIE1B;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetOCIEAValue(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef OCIE0A
  case 0:
    return OCIE0A;
#endif
#ifdef OCIE1A
  case 1:
    return OCIE1A;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetTOIEValue(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef TOIE0
  case 0:
    return TOIE0;
#endif
#ifdef TOIE1
  case 1:
    return TOIE1;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetOCFBValue(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef OCF0B
  case 0:
    return OCF0B;
#endif
#ifdef OCF1B
  case 1:
    return OCF1B;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetOCFAValue(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef OCF0A
  case 0:
    return OCF0A;
#endif
#ifdef OCF1A
  case 1:
    return OCF1A;
#endif
  default:
    return 0; // Invalid timer ID
  }
}

static auto GetTOVValue(uint8_t timerId) -> uint8_t {
  switch (timerId) {
#ifdef TOV0
  case 0:
    return TOV0;
#endif
#ifdef TOV1
  case 1:
    return TOV1;
#endif
  default:
    return 0; // Invalid timer ID
  }
}
