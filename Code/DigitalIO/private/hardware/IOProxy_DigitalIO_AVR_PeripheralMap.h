#pragma once

#include <avr/io.h>

static auto GetDDRAddress(BANK_LETTER bankLetter) -> volatile uint8_t * {
  switch (bankLetter) {
#ifdef DDRA
  case BANK_LETTER::A:
    return &DDRA;
#endif
#ifdef DDRB
  case BANK_LETTER::B:
    return &DDRB;
#endif
#ifdef DDRC
  case BANK_LETTER::C:
    return &DDRC;
#endif
#ifdef DDRD
  case BANK_LETTER::D:
    return &DDRD;
#endif
  default:
    return nullptr; // Invalid bank letter
  }
}

static auto GetPORTAddress(BANK_LETTER bankLetter) -> volatile uint8_t * {
  switch (bankLetter) {
#ifdef PORTA
  case BANK_LETTER::A:
    return &PORTA;
#endif
#ifdef PORTB
  case BANK_LETTER::B:
    return &PORTB;
#endif
#ifdef PORTC
  case BANK_LETTER::C:
    return &PORTC;
#endif
#ifdef PORTD
  case BANK_LETTER::D:
    return &PORTD;
#endif
  default:
    return nullptr; // Invalid bank letter
  }
}

static auto GetPINAddress(BANK_LETTER bankLetter) -> volatile uint8_t * {
  switch (bankLetter) {
#ifdef PINA
  case BANK_LETTER::A:
    return &PINA;
#endif
#ifdef PINB
  case BANK_LETTER::B:
    return &PINB;
#endif
#ifdef PINC
  case BANK_LETTER::C:
    return &PINC;
#endif
#ifdef PIND
  case BANK_LETTER::D:
    return &PIND;
#endif
  default:
    return nullptr; // Invalid bank letter
  }
};