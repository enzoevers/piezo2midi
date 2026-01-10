#include <gtest/gtest.h>

#include "ADC/MCP3008.h"

class TestMCP3008 : public ::testing::Test {
public:
  MCP3008 adcSampler;
};

TEST_F(TestMCP3008, AdcResolutionReturns1024) {
  EXPECT_EQ(adcSampler.AdcResolution(), 1024);
}