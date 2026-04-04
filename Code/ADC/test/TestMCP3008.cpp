#include "ADC/MCP3008/MCP3008.h"
#include "Communication/SPI/mocks/Mock_ISPIMaster.h"
#include <gtest/gtest.h>
#include <memory>

class Test_MCP3008_Fixture : public ::testing::Test {
public:
  Test_MCP3008_Fixture()
      : m_mockSPIMaster(std::make_unique<Mock_ISPIMaster>()),
        m_mcp3008(std::make_unique<MCP3008>(*m_mockSPIMaster)) {}

  std::unique_ptr<Mock_ISPIMaster> m_mockSPIMaster;
  std::unique_ptr<MCP3008> m_mcp3008;
};

//==============================
// AdcResolution()
//==============================

TEST_F(Test_MCP3008_Fixture, AdcResolution_Returns1024) {
  EXPECT_EQ(m_mcp3008->AdcResolution(), 1024);
}

//==============================
// ReadChannel(...)
//==============================

TEST_F(Test_MCP3008_Fixture, ReadChannel_Returns0_WhenChannelOutOfRange) {
  EXPECT_EQ(m_mcp3008->ReadChannel(8),
            0); // Channel 8 is out of range for MCP3008
}

TEST_F(Test_MCP3008_Fixture,
       ReadChannel_SendExpectedSPICommandsAndReturnsExpectedValue_Channel2) {

  static constexpr uint16_t expectedAdcValue =
      0b10'1010'0111; // Random 10-bit value to be returned in the test

  {
    testing::InSequence s; // Ensure the order of operations

    // See section "6.0 APPLICATIONS INFORMATION" in the MCP3008 datasheet for
    // the expected SPI commands

    EXPECT_CALL(*m_mockSPIMaster,
                SendByte(0b0000'0001)) // Start bit
        .Times(1)
        .WillOnce(
            testing::Return(0x00)); // First byte sent, response is ignored

    EXPECT_CALL(*m_mockSPIMaster,
                SendByte(0b1010'0000)) // Single-ended mode, channel 2
        .Times(1)
        .WillOnce(
            testing::Return((expectedAdcValue >> 8) &
                            0b011)); // Second byte sent, response contains the
                                     // 2 most significant bits of the result

    EXPECT_CALL(
        *m_mockSPIMaster,
        SendByte(0b0000'0000)) // Third byte sent, response contains the 8
                               // least significant bits of the result
        .Times(1)
        .WillOnce(testing::Return(
            expectedAdcValue & 0xFF)); // Third byte sent, response contains the
                                       // 8 least significant bits of the result
  }

  EXPECT_EQ(m_mcp3008->ReadChannel(2), expectedAdcValue);
}
