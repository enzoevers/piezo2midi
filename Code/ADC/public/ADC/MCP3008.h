#pragma once

#include "ADC/IAdcSampler.h"
#include <stdint.h>

class MCP3008 : public IAdcSampler<uint16_t> {
public:
  ~MCP3008() = default;

  auto AdcResolution() -> uint16_t override;
  auto ReadChannel(uint8_t channel) -> uint16_t override;
};