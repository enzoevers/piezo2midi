#pragma once

#include <stdint.h>

template <typename SampleReturnType> class IAdcSampler {
public:
#ifndef __AVR__
  virtual ~IAdcSampler() = default;
#endif

  virtual auto AdcResolution() -> SampleReturnType = 0;
  virtual auto ReadChannel(uint8_t channel) -> SampleReturnType = 0;
};