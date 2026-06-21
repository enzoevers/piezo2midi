#pragma once

#include "ChannelMapping/IVelocityMap.h"

/**
 * Configuration for the velocity mapping function.
 *
 * a = alpha (most likely in the range [0.995, 1.005])
 * r = max value
 * x = input velocity (range: [1, maxValue])])
 *
 * y = ((r-1) / (a^(r-1) - 1)) * (a^(x-1) - 1) + 1
 *     \---------------------/   \-----------/
 *                A                    B
 *
 * a == 1 -> don't use the formula, just return y = x (linear mapping)
 * a > 1 -> y <= x (exponential: mapping that produces softer velocities)
 * a < 1 -> y >= x (logarithmic: mapping that produces harder velocities)
 */
class VelocityMap : public IVelocityMap {
public:
  VelocityMap(uint16_t maxValue, float alpha);

  //== IVelocityMap implementation ==//
  auto MapVelocity(uint16_t velocity) const -> uint16_t override;
  //== End of IVelocityMap implementation ==//

  void SetAlpha(float alpha);
  auto GetAlpha() const -> float;

private:
  const uint16_t m_maxValue{};
  float m_alpha{};
  float m_precomputedA{};
};