#include "DigitalIO/DigitalIO_AVR.h"
#include "DigitalIO/IOProxy_DigitalIO_AVR.h"

#include "Communication/UART/IOProxy_UART_AVR.h"
#include "Communication/UART/UART_AVR.h"

#include <stddef.h>

int main() {
  IOProxy_DigitalIO_AVR ioProxy_digitialIO_bankB(BANK_LETTER::B);
  DigitalIO_AVR digitalIO_B(ioProxy_digitialIO_bankB);

  // Arduino UNO build-in LED is on bank B pin 5
  const uint8_t pin5 = 5;
  digitalIO_B.SetMode(pin5, PIN_MODE::OUTPUT);

  IOProxy_UART_AVR ioProxy_uart0(0);
  UART_AVR uart0(ioProxy_uart0);

  const char message[] =
      "Send 'l' to turn off the LED\n\rSend 'h' to turn on the LED\n\r";

  for (size_t i = 0; message[i] != '\0'; ++i) {
    uart0.SendByte(message[i]);
  }

  while (true) {
    if (uart0.ByteAvailable()) {
      const char charReceived = uart0.ReadByte_NonBlocking();
      const char logMessage[] = "Received char: ";
      for (size_t i = 0; logMessage[i] != '\0'; ++i) {
        uart0.SendByte(logMessage[i]);
      }
      uart0.SendByte(charReceived);
      uart0.SendByte('\n');
      uart0.SendByte('\r');

      if (charReceived == 'l') {
        digitalIO_B.SetState(pin5, PIN_STATE::LOW);
      } else if (charReceived == 'h') {
        digitalIO_B.SetState(pin5, PIN_STATE::HIGH);
      }
    }
  }

  return 0;
}