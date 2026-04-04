#include "DigitalIO/DigitalIO_AVR.h"
#include "DigitalIO/IOProxy_DigitalIO_AVR.h"

#include "Communication/UART/IOProxy_UART_AVR.h"
#include "Communication/UART/UART_AVR.h"

#include "Communication/SPI/IOProxy_SPI_AVR.h"
#include "Communication/SPI/SPIMaster_AVR.h"

#include <stddef.h>

int main() {
  IOProxy_DigitalIO_AVR ioProxy_digitialIO_bankD(BANK_LETTER::D);
  DigitalIO_AVR digitalIO_D(ioProxy_digitialIO_bankD);

  const uint8_t pinLED = 7;
  digitalIO_D.SetMode(pinLED, PIN_MODE::OUTPUT);

  IOProxy_UART_AVR ioProxy_uart0(0);
  UART_AVR uart0(ioProxy_uart0);

  IOProxy_DigitalIO_AVR ioProxy_digitialIO_bankB(BANK_LETTER::B);
  DigitalIO_AVR digitalIO_B(ioProxy_digitialIO_bankB);

  // For this test app it is assumed
  // that a loopback connection is made between MOSI and MISO pins.
  PinControl slavePins[] = {{digitalIO_B, 2}};
  SPIPinConfig spiPinConfig{.MOSI_pin = {digitalIO_B, 3},
                            .MISO_pin = {digitalIO_B, 4},
                            .SCLK_pin = {digitalIO_B, 5},
                            .SS_pins = slavePins,
                            .slaveCount = 1};
  IOProxy_SPI_AVR ioProxy_spi0(0);
  SPIMaster_AVR spiMaster0(ioProxy_spi0, spiPinConfig);

  const char message[] =
      "Send 'l' to turn off the LED\n\rSend 'h' to turn on the LED\n\r";

  for (size_t i = 0; message[i] != '\0'; ++i) {
    uart0.SendByte(message[i]);
  }

  while (true) {
    if (uart0.ByteAvailable()) {
      const char charReceived = uart0.ReadByte_NonBlocking();
      const char logMessage[] = "Received UART char: ";
      for (size_t i = 0; logMessage[i] != '\0'; ++i) {
        uart0.SendByte(logMessage[i]);
      }
      uart0.SendByte(charReceived);
      uart0.SendByte('\n');
      uart0.SendByte('\r');

      if (charReceived == 'l') {
        digitalIO_D.SetState(pinLED, PIN_STATE::LOW);
      } else if (charReceived == 'h') {
        digitalIO_D.SetState(pinLED, PIN_STATE::HIGH);
      }

      const uint8_t receivedSpiByte = spiMaster0.SendByte(charReceived);
      const char spiLogMessage[] = "Received SPI char: ";
      for (size_t i = 0; spiLogMessage[i] != '\0'; ++i) {
        uart0.SendByte(spiLogMessage[i]);
      }
      uart0.SendByte(receivedSpiByte);
      uart0.SendByte('\n');
      uart0.SendByte('\r');
    }
  }

  return 0;
}