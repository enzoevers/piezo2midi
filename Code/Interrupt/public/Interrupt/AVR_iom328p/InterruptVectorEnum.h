#pragma once

enum class INTERRUPT_VECTORS_iom328p {
  IV_RESET,
  IV_INT0,
  IV_INT1,
  IV_PCINT0,
  IV_PCINT1,
  IV_PCINT2,
  IV_WDT,
  IV_TIMER2_COMPA,
  IV_TIMER2_COMPB,
  IV_TIMER2_OVF,
  IV_TIMER1_CAPT,
  IV_TIMER1_COMPA,
  IV_TIMER1_COMPB,
  IV_TIMER1_OVF,
  IV_TIMER0_COMPA,
  IV_TIMER0_COMPB,
  IV_TIMER0_OVF,
  IV_SPI_STC,
  IV_USART_RX,
  IV_USART_UDRE,
  IV_USART_TX,
  IV_ADC,
  IV_EE_READY,
  IV_ANALOG_COMP,
  IV_TWI,
  IV_SPM_READY,

  SIZE, // This is not a valid interrupt vector, it is used to get the size of
        // the enum
};