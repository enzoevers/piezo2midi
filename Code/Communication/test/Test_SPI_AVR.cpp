#include "Communication/SPI/SPIMaster_AVR.h"
#include "Communication/SPI/mocks/Mock_IOProxy_SPI_AVR.h"
#include "DigitalIO/mocks/Mock_IDigitalIO.h"
#include <array>
#include <gtest/gtest.h>
#include <memory>

class Test_SPI_AVR_fixture : public ::testing::Test {
public:
  Test_SPI_AVR_fixture()
      : m_mockIOProxy(std::make_unique<Mock_IOProxy_SPI_AVR>()),
        m_mockDigitalIO(std::make_unique<Mock_IDigitalIO>()),
        m_SS_pins{PinControl{*m_mockDigitalIO, 4},
                  PinControl{*m_mockDigitalIO, 5}},
        m_spiPinConfig(std::make_unique<SPIPinConfig>(SPIPinConfig{
            .MOSI_pin = {*m_mockDigitalIO, 1},
            .MISO_pin = {*m_mockDigitalIO, 2},
            .SCLK_pin = {*m_mockDigitalIO, 3},
            .SS_pins = m_SS_pins.data(),
            .slaveCount = static_cast<uint8_t>(m_SS_pins.size())})),
        m_spi_AVR(
            std::make_unique<SPIMaster_AVR>(*m_mockIOProxy, *m_spiPinConfig)) {
    SetupReadDefineMocks();
  }

  void SetupReadDefineMocks() {
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_SPE))
        .WillByDefault(
            testing::Return(6)); // Assume that DEF_SPE is bit 6 in SPCRx
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_DORD))
        .WillByDefault(
            testing::Return(5)); // Assume that DEF_DORD is bit 5 in SPCRx
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_MSTR))
        .WillByDefault(
            testing::Return(4)); // Assume that DEF_MSTR is bit 4 in SPCRx
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_CPOL))
        .WillByDefault(
            testing::Return(3)); // Assume that DEF_CPOL is bit 3 in SPCRx
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_CPHA))
        .WillByDefault(
            testing::Return(2)); // Assume that DEF_CPHA is bit 2 in SPCRx
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_SPR1))
        .WillByDefault(
            testing::Return(1)); // Assume that DEF_SPR1 is bit 1 in SPCRx
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_SPR0))
        .WillByDefault(
            testing::Return(0)); // Assume that DEF_SPR0 is bit 0 in SPCRx
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_SPIF))
        .WillByDefault(
            testing::Return(7)); // Assume that DEF_SPIF is bit 7 in SPSRx
    ON_CALL(*m_mockIOProxy, ReadDefine(SPI_AVR_DefineEnum::DEF_SPI2X))
        .WillByDefault(
            testing::Return(0)); // Assume that DEF_SPI2X is bit 0 in SPSRx
  }

  std::unique_ptr<Mock_IOProxy_SPI_AVR> m_mockIOProxy;
  std::unique_ptr<Mock_IDigitalIO> m_mockDigitalIO;
  std::array<PinControl, 2> m_SS_pins;
  std::unique_ptr<SPIPinConfig> m_spiPinConfig;
  std::unique_ptr<SPIMaster_AVR> m_spi_AVR;
};

//==============================
// SetMode(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, SetMode_SetsModeForMode0) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x0F));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'0011;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->SetMode(SPI_MODE::MODE0);
}

TEST_F(Test_SPI_AVR_fixture, SetMode_SetsModeForMode1) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x0F));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'0111;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->SetMode(SPI_MODE::MODE1);
}

TEST_F(Test_SPI_AVR_fixture, SetMode_SetsModeForMode2) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x0F));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'1011;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->SetMode(SPI_MODE::MODE2);
}

TEST_F(Test_SPI_AVR_fixture, SetMode_SetsModeForMode3) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x00));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'1100;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->SetMode(SPI_MODE::MODE3);
}

//==============================
// SetClockFrequency(...)
//
// This assumes an F_CPU of 16 MHz.
//==============================

TEST_F(Test_SPI_AVR_fixture,
       SetClockFrequency_UpdatesClockFrequency_WhenDividerIsExcact) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x00));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0x00));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'0001;
  static constexpr uint8_t expectedSPSRxValue = 0b0000'0001;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPSRx,
                                            expectedSPSRxValue))
      .Times(1);

  m_spi_AVR->SetClockFrequency(2'000'000);
}

TEST_F(Test_SPI_AVR_fixture,
       SetClockFrequency_UpdatesClockFrequency_WhenDividerIsNotExact_roundUp) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x0F));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0x0F));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'1100;
  static constexpr uint8_t expectedSPSRxValue = 0b0000'1110;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPSRx,
                                            expectedSPSRxValue))
      .Times(1);

  m_spi_AVR->SetClockFrequency(3'600'000, ROUND_DIRECTION::UP);
}

TEST_F(
    Test_SPI_AVR_fixture,
    SetClockFrequency_UpdatesClockFrequency_WhenDividerIsNotExact_roundDown) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x0F));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0x00));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'1101;
  static constexpr uint8_t expectedSPSRxValue = 0b0000'0001;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPSRx,
                                            expectedSPSRxValue))
      .Times(1);

  m_spi_AVR->SetClockFrequency(3'600'000, ROUND_DIRECTION::DOWN);
}

TEST_F(
    Test_SPI_AVR_fixture,
    SetClockFrequency_UpdatesClockFrequency_WhenDividerIsNotExact_roundClosest_roundsUp) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x0F));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0x0F));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'1100;
  static constexpr uint8_t expectedSPSRxValue = 0b0000'1110;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPSRx,
                                            expectedSPSRxValue))
      .Times(1);

  m_spi_AVR->SetClockFrequency(3'600'000, ROUND_DIRECTION::CLOSEST);
}

TEST_F(
    Test_SPI_AVR_fixture,
    SetClockFrequency_UpdatesClockFrequency_WhenDividerIsNotExact_roundClosest_roundsDown) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x0F));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0x00));

  static constexpr uint8_t expectedSPCRxValue = 0b0000'1101;
  static constexpr uint8_t expectedSPSRxValue = 0b0000'0001;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPSRx,
                                            expectedSPSRxValue))
      .Times(1);

  m_spi_AVR->SetClockFrequency(2'600'000, ROUND_DIRECTION::CLOSEST);
}

//==============================
// SetDataOrder(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, SetDataOrder_SetsDataOrderToMSBFirst) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0xFF));

  static constexpr uint8_t expectedSPCRxValue = 0b1101'1111;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->SetDataOrder(SPI_DATAORDER::MSB_FIRST);
}

TEST_F(Test_SPI_AVR_fixture, SetDataOrder_SetsDataOrderToLSBFirst) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x00));

  static constexpr uint8_t expectedSPCRxValue = 0b0010'0000;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->SetDataOrder(SPI_DATAORDER::LSB_FIRST);
}

//==============================`
// GetMode(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, GetMode_ReturnsCurrentMode_mode0) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0000));

  EXPECT_EQ(m_spi_AVR->GetMode(), SPI_MODE::MODE0);
}

TEST_F(Test_SPI_AVR_fixture, GetMode_ReturnsCurrentMode_mode1) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0100));

  EXPECT_EQ(m_spi_AVR->GetMode(), SPI_MODE::MODE1);
}

TEST_F(Test_SPI_AVR_fixture, GetMode_ReturnsCurrentMode_mode2) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'1000));

  EXPECT_EQ(m_spi_AVR->GetMode(), SPI_MODE::MODE2);
}

TEST_F(Test_SPI_AVR_fixture, GetMode_ReturnsCurrentMode_mode3) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'1100));

  EXPECT_EQ(m_spi_AVR->GetMode(), SPI_MODE::MODE3);
}

//==============================
// GetClockFrequency(...)
//
// Assuming an F_CPU of 16 MHz
//==============================

TEST_F(Test_SPI_AVR_fixture,
       GetClockFrequency_ReturnsCurrentClockFrequency_div2) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0000));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0b0000'0001));

  EXPECT_EQ(m_spi_AVR->GetClockFrequency(), 8'000'000);
}

TEST_F(Test_SPI_AVR_fixture,
       GetClockFrequency_ReturnsCurrentClockFrequency_div4) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0000));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0b0000'0000));

  EXPECT_EQ(m_spi_AVR->GetClockFrequency(), 4'000'000);
}

TEST_F(Test_SPI_AVR_fixture,
       GetClockFrequency_ReturnsCurrentClockFrequency_div8) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0001));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0b0000'0001));

  EXPECT_EQ(m_spi_AVR->GetClockFrequency(), 2'000'000);
}

TEST_F(Test_SPI_AVR_fixture,
       GetClockFrequency_ReturnsCurrentClockFrequency_div16) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0001));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0b0000'0000));

  EXPECT_EQ(m_spi_AVR->GetClockFrequency(), 1'000'000);
}

TEST_F(Test_SPI_AVR_fixture,
       GetClockFrequency_ReturnsCurrentClockFrequency_div32) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0010));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0b0000'0001));

  EXPECT_EQ(m_spi_AVR->GetClockFrequency(), 500'000);
}

TEST_F(Test_SPI_AVR_fixture,
       GetClockFrequency_ReturnsCurrentClockFrequency_div64_UsingSPI2X) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0011));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0b0000'0001));

  EXPECT_EQ(m_spi_AVR->GetClockFrequency(), 250'000);
}

TEST_F(Test_SPI_AVR_fixture,
       GetClockFrequency_ReturnsCurrentClockFrequency_div64_NotUsingSPI2X) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0010));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0b0000'0000));

  EXPECT_EQ(m_spi_AVR->GetClockFrequency(), 250'000);
}

TEST_F(Test_SPI_AVR_fixture,
       GetClockFrequency_ReturnsCurrentClockFrequency_div128) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0011));
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
      .WillByDefault(testing::Return(0b0000'0000));

  EXPECT_EQ(m_spi_AVR->GetClockFrequency(), 125'000);
}

//==============================
// GetDataOrder(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, GetDataOrder_ReturnsCurrentDataOrder_MSBFirst) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0000'0000));

  EXPECT_EQ(m_spi_AVR->GetDataOrder(), SPI_DATAORDER::MSB_FIRST);
}

TEST_F(Test_SPI_AVR_fixture, GetDataOrder_ReturnsCurrentDataOrder_LSBFirst) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0b0010'0000));

  EXPECT_EQ(m_spi_AVR->GetDataOrder(), SPI_DATAORDER::LSB_FIRST);
}

//==============================
// SetPinModes(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, SetPinModes_SetModeCorrectlyAndDriveSSPinHigh) {
  EXPECT_CALL(*m_mockDigitalIO,
              SetMode(m_spiPinConfig->MOSI_pin.pin, PIN_MODE::OUTPUT))
      .Times(1);
  EXPECT_CALL(*m_mockDigitalIO,
              SetMode(m_spiPinConfig->MISO_pin.pin, PIN_MODE::INPUT))
      .Times(0); // MISO direction is set by the hardware
  EXPECT_CALL(*m_mockDigitalIO,
              SetMode(m_spiPinConfig->SCLK_pin.pin, PIN_MODE::OUTPUT))
      .Times(1);
  EXPECT_CALL(*m_mockDigitalIO,
              SetMode(m_spiPinConfig->SS_pins[0].pin, PIN_MODE::OUTPUT))
      .Times(1);
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[0].pin, PIN_STATE::HIGH))
      .Times(1);
  EXPECT_CALL(*m_mockDigitalIO,
              SetMode(m_spiPinConfig->SS_pins[1].pin, PIN_MODE::OUTPUT))
      .Times(1);
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[1].pin, PIN_STATE::HIGH))
      .Times(1);

  m_spi_AVR->SetPinModes();
}

//==============================
// SetMasterMode(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, SetMasterMode_SetsMasterMode) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x00));

  static constexpr uint8_t expectedSPCRxValue = 0b0001'0000;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->SetMasterMode();
}

//==============================
// EnableSPI(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, EnableSPI_EnablesSPI) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0x00));

  static constexpr uint8_t expectedSPCRxValue = 0b0100'0000;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->EnableSPI();
}

//==============================
// DisableSPI(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, DisableSPI_DisablesSPI) {
  ON_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPCRx))
      .WillByDefault(testing::Return(0xFF));

  static constexpr uint8_t expectedSPCRxValue = 0b1011'1111;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(SPI_AVR_RegisterEnum::REG_SPCRx,
                                            expectedSPCRxValue))
      .Times(1);

  m_spi_AVR->DisableSPI();
}

//==============================
// SelectSlave(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, SelectSlave_ReturnFalse_WhenSlaveIndexIsInvalid) {
  EXPECT_FALSE(m_spi_AVR->SelectSlave(2));
}

TEST_F(
    Test_SPI_AVR_fixture,
    SelectSlave_ReturnTrueAndDriveSSPinLow_DoesNotChangeOtherSlaveSelects_WhenSlaveIndexIsValid_Index0) {
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[0].pin, PIN_STATE::LOW))
      .Times(1);
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[1].pin, testing::_))
      .Times(0);

  EXPECT_TRUE(m_spi_AVR->SelectSlave(0));
}

TEST_F(
    Test_SPI_AVR_fixture,
    SelectSlave_ReturnTrueAndDriveSSPinLow_DoesNotChangeOtherSlaveSelects_WhenSlaveIndexIsValid_Index1) {
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[0].pin, testing::_))
      .Times(0);
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[1].pin, PIN_STATE::LOW))
      .Times(1);

  EXPECT_TRUE(m_spi_AVR->SelectSlave(1));
}

//==============================
// DeselectSlave(...)
//==============================

TEST_F(Test_SPI_AVR_fixture,
       DeselectSlave_ReturnFalse_WhenSlaveIndexIsInvalid) {
  EXPECT_FALSE(m_spi_AVR->DeselectSlave(2));
}

TEST_F(
    Test_SPI_AVR_fixture,
    DeselectSlave_ReturnTrueAndDriveSSPinHigh_DoesNotChangeOtherSlaveSelects_WhenSlaveIndexIsValid_Index0) {
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[0].pin, PIN_STATE::HIGH))
      .Times(1);
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[1].pin, testing::_))
      .Times(0);

  EXPECT_TRUE(m_spi_AVR->DeselectSlave(0));
}

TEST_F(
    Test_SPI_AVR_fixture,
    DeselectSlave_ReturnTrueAndDriveSSPinHigh_DoesNotChangeOtherSlaveSelects_WhenSlaveIndexIsValid_Index1) {
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[0].pin, testing::_))
      .Times(0);
  EXPECT_CALL(*m_mockDigitalIO,
              SetState(m_spiPinConfig->SS_pins[1].pin, PIN_STATE::HIGH))
      .Times(1);

  EXPECT_TRUE(m_spi_AVR->DeselectSlave(1));
}

//==============================
// SendByte(...)
//==============================

TEST_F(Test_SPI_AVR_fixture, SendByte_WritesByteToSPDRAndWaitsForTransmission) {
  static constexpr uint8_t byteToSend = 0xAB;
  static constexpr uint8_t byteReceived = 0xCD;

  {
    testing::InSequence s; // Ensure the order of operations
    EXPECT_CALL(*m_mockIOProxy,
                WriteRegister(SPI_AVR_RegisterEnum::REG_SPDRx, byteToSend))
        .Times(1);
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPSRx))
        .Times(2)
        .WillOnce(testing::Return(0x00)) // Not ready yet
        .WillOnce(testing::Return(1 << m_mockIOProxy->ReadDefine(
                                      SPI_AVR_DefineEnum::DEF_SPIF))); // Ready
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(SPI_AVR_RegisterEnum::REG_SPDRx))
        .Times(1)
        .WillOnce(testing::Return(byteReceived));
  }

  const uint8_t result = m_spi_AVR->SendByte(byteToSend);

  EXPECT_EQ(result, byteReceived);
}
