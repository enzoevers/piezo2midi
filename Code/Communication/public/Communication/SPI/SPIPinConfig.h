#pragma once

#include "DigitalIO/PinControl.h"

struct SPIPinConfig {
  PinControl MOSI_pin;
  PinControl MISO_pin;
  PinControl SCLK_pin;
  PinControl *SS_pins;
  uint8_t slaveCount;
};