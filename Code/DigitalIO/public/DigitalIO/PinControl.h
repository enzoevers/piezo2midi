#pragma once

#include "DigitalIO/IDigitalIO.h"

struct PinControl {
  IDigitalIO &digitalIO;
  uint8_t pin;
};
