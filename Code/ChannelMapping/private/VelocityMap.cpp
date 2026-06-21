#include "ChannelMapping/VelocityMap.h"

#include <math.h>

auto float_eq(float a, float b) -> bool {
  static constexpr float epsilon = 0.0001f;
  return (a - b) < epsilon && (b - a) < epsilon;
}

VelocityMap::VelocityMap(uint16_t maxValue, float alpha)
    : m_maxValue(maxValue) {
  SetAlpha(alpha);
}

auto VelocityMap::MapVelocity(uint16_t velocity) const -> uint16_t {
  if (float_eq(m_alpha, 1.0f)) {
    return velocity;
  }

  if (velocity == 0) {
    return 0;
  }

  if (velocity >= m_maxValue) {
    return m_maxValue;
  }

  float B = powf(m_alpha, velocity - 1.f) - 1.f;
  return static_cast<uint16_t>(m_precomputedA * B + 1.f);
}

void VelocityMap::SetAlpha(float alpha) {
  m_alpha = alpha;
  m_precomputedA = (m_maxValue - 1.f) / (powf(m_alpha, m_maxValue - 1.f) - 1.f);
}

auto VelocityMap::GetAlpha() const -> float { return m_alpha; }