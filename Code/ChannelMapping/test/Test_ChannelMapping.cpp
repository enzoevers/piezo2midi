#include <gtest/gtest.h>
#include <memory>

#include "ChannelMapping/VelocityMap.h"

class TestVelocityMap_fixture : public ::testing::Test {
public:
  TestVelocityMap_fixture()
      : m_velocityMap(
            std::make_unique<VelocityMap>(m_defaultMaxValue, m_defaultAlpha)) {}

  std::unique_ptr<VelocityMap> m_velocityMap;
  static constexpr uint16_t m_defaultMaxValue = 1023;
  static constexpr float m_defaultAlpha = 1.001f;
};

//==============================
// Construction
//==============================

TEST_F(TestVelocityMap_fixture, test_ConstructionSetsAlpha) {
  EXPECT_FLOAT_EQ(m_velocityMap->GetAlpha(), m_defaultAlpha);
}

//==============================
// SetAlpha(...) & GetAlpha()
//==============================

TEST_F(TestVelocityMap_fixture, test_SetGetAlpha) {
  static constexpr float newAlpha = 0.995f;

  m_velocityMap->SetAlpha(newAlpha);
  EXPECT_FLOAT_EQ(m_velocityMap->GetAlpha(), newAlpha);
}

//==============================
// MapVelocity(...)
//==============================

TEST_F(TestVelocityMap_fixture,
       test_MapVelocity_ReturnLinearMappingForAlphaOne) {
  m_velocityMap->SetAlpha(1.0f);

  EXPECT_EQ(m_velocityMap->MapVelocity(1), 1);

  EXPECT_EQ(m_velocityMap->MapVelocity(m_defaultMaxValue / 2),
            m_defaultMaxValue / 2);

  EXPECT_EQ(m_velocityMap->MapVelocity(m_defaultMaxValue), m_defaultMaxValue);
}

TEST_F(TestVelocityMap_fixture,
       test_MapVelocity_ReturnZeroForAnyAlphaAndVelocityZero) {
  m_velocityMap->SetAlpha(0.995f);
  EXPECT_EQ(m_velocityMap->MapVelocity(0), 0);

  m_velocityMap->SetAlpha(1.0f);
  EXPECT_EQ(m_velocityMap->MapVelocity(0), 0);

  m_velocityMap->SetAlpha(1.005f);
  EXPECT_EQ(m_velocityMap->MapVelocity(0), 0);
}

TEST_F(TestVelocityMap_fixture,
       test_MapVelocity_ReturnMaxValueForAnyAlphaAndVelocityMaxValue) {
  m_velocityMap->SetAlpha(0.995f);
  EXPECT_EQ(m_velocityMap->MapVelocity(m_defaultMaxValue), m_defaultMaxValue);

  m_velocityMap->SetAlpha(1.0f);
  EXPECT_EQ(m_velocityMap->MapVelocity(m_defaultMaxValue), m_defaultMaxValue);

  m_velocityMap->SetAlpha(1.005f);
  EXPECT_EQ(m_velocityMap->MapVelocity(m_defaultMaxValue), m_defaultMaxValue);
}

TEST_F(TestVelocityMap_fixture,
       test_MapVelocity_ReturnsValueSmallerThanInputForAlphaGreaterThanOne) {
  m_velocityMap->SetAlpha(1.005f);
  EXPECT_LT(m_velocityMap->MapVelocity(m_defaultMaxValue / 2),
            m_defaultMaxValue / 2);
}

TEST_F(TestVelocityMap_fixture,
       test_MapVelocity_ReturnsValueGreaterThanInputForAlphaLessThanOne) {
  m_velocityMap->SetAlpha(0.995f);
  EXPECT_GT(m_velocityMap->MapVelocity(m_defaultMaxValue / 2),
            m_defaultMaxValue / 2);
}
