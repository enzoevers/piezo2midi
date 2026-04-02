#include "ADC/MCP3008.h"

auto MCP3008::AdcResolution() -> uint16_t {
  return 1024; // 10-bit ADC has a resolution of 1024 discrete values (0-1023)
}

auto MCP3008::ReadChannel([[maybe_unused]] uint8_t channel) -> uint16_t {
  return 0;
}