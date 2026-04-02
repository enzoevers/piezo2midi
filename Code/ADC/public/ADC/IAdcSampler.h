#pragma once

#include <stdint.h>

template <typename SampleReturnType> class IAdcSampler {
public:
  // virtual ~IAdcSampler() = default; // AVR has an issue not not having
  // libstdc++ linked in, so we can't have a virtual destructor.

  virtual auto AdcResolution() -> SampleReturnType = 0;
  virtual auto ReadChannel(uint8_t channel) -> SampleReturnType = 0;
};