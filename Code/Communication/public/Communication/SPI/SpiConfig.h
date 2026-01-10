#pragma once

#include "DigitalIO/PinName.h"
#include <stdint.h>

enum class SPI_MODE { MODE0, MODE1, MODE2, MODE3 };

struct SpiConfig {
  PinName MOSI_pin;
  PinName MISO_pin;
  PinName SS_pin;
  PinName SCLK_pin;
  uint32_t CLK_hz;
  SPI_MODE spiMode;
};