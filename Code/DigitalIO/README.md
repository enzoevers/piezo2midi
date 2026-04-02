# Usage

```cpp
#include "DigitalIO/DigitalIO_AVR.h"
#include "DigitalIO/IOProxy_DigitalIO_AVR.h"

int main() {
    IOProxy_DigitalIO_AVR ioProxy_digitialIO_bankB(BANK_LETTER::B);
    DigitalIO_AVR digitalIOB(ioProxy_digitialIO_bankB);

    // Arduino UNO build-in LED is on bank B pin 5
    const uint8_t pin = 5;

    digitalIOB.SetMode(pin, PIN_MODE::OUTPUT);
    digitalIOB.SetState(pin, PIN_STATE::HIGH);

    return 0;
}
```
