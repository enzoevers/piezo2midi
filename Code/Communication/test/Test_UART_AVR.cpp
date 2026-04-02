#include <gtest/gtest.h>
#include <memory>

#include "Communication/UART/UART_AVR.h"
#include "Communication/UART/mocks/Mock_IOProxy_UART_AVR.h"

class Test_UART_AVR_fixture : public ::testing::Test {
public:
  Test_UART_AVR_fixture() {
    m_mockIOProxy = std::make_unique<Mock_IOProxy_UART_AVR>();
    m_uart_AVR = std::make_unique<UART_AVR>(*m_mockIOProxy);
  }

  std::unique_ptr<Mock_IOProxy_UART_AVR> m_mockIOProxy;
  std::unique_ptr<UART_AVR> m_uart_AVR;
};

//==============================
// Constructor
//==============================

TEST_F(Test_UART_AVR_fixture, Constructor_SetsDefaultBaudRateAndFrameFormat) {
  UART_FrameFormat expectedFrameFormat{UART_DataBits::DB8, UART_Parity::NONE,
                                       UART_StopBits::SB1};

  EXPECT_EQ(m_uart_AVR->GetBaudRate(), 9600);
  EXPECT_EQ(m_uart_AVR->GetFrameFormat(), expectedFrameFormat);
  EXPECT_EQ(m_uart_AVR->GetUARTMode(), UART_Mode::ASYNCHRONOUS);
}

TEST_F(Test_UART_AVR_fixture, Constructor_SetsCustomBaudRateAndFrameFormat) {
  UART_FrameFormat customFrameFormat{UART_DataBits::DB7, UART_Parity::EVEN,
                                     UART_StopBits::SB2};

  UART_AVR customUART(*m_mockIOProxy, 115200, customFrameFormat,
                      UART_Mode::SYNCHRONOUS);

  EXPECT_EQ(customUART.GetBaudRate(), 115200);
  EXPECT_EQ(customUART.GetFrameFormat(), customFrameFormat);
  EXPECT_EQ(customUART.GetUARTMode(), UART_Mode::SYNCHRONOUS);
}

//==============================
// SetBaudRate(...)
//==============================

TEST_F(Test_UART_AVR_fixture,
       SetBaudRate_SetsCorrectPrescaler_for9600BaudAt16MHz) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
      .WillByDefault(testing::Return(0xFF));
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_U2Xn))
      .WillByDefault(
          testing::Return(4)); // Assume that DEF_U2Xn is bit 4 in UCSRnA

  static constexpr uint8_t expectedUBRRnH = 0;
  static constexpr uint8_t expectedUBRRnL = 0b0110'0111; // 103 in decimal
  static constexpr uint8_t expectedUSARnA =
      0b1110'1111; // Check that the U2X bit is set to 0

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(UART_AVR_RegisterEnum::REG_UBRRnH, expectedUBRRnH))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(UART_AVR_RegisterEnum::REG_UBRRnL, expectedUBRRnL))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnA, expectedUSARnA))
      .Times(1);

  m_uart_AVR->SetBaudRate(9600);
  EXPECT_EQ(m_uart_AVR->GetBaudRate(), 9600);
}

TEST_F(Test_UART_AVR_fixture,
       SetBaudRate_SetsCorrectPrescaler_for115200BaudAt16MHz) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
      .WillByDefault(testing::Return(0x00));
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_U2Xn))
      .WillByDefault(
          testing::Return(2)); // Assume that DEF_U2Xn is bit 2 in UCSRnA

  static constexpr uint8_t expectedUBRRnH = 0;
  static constexpr uint8_t expectedUBRRnL = 0b0001'0000; // 16 in decimal
  static constexpr uint8_t expectedUSARnA =
      0b0000'0100; // Check that the U2X bit is set to 1

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(UART_AVR_RegisterEnum::REG_UBRRnH, expectedUBRRnH))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(UART_AVR_RegisterEnum::REG_UBRRnL, expectedUBRRnL))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnA, expectedUSARnA))
      .Times(1);

  m_uart_AVR->SetBaudRate(115200);
  EXPECT_EQ(m_uart_AVR->GetBaudRate(), 115200);
}

//==============================
// SetFrameFormat(...)
//
// The default frame format is:
// - 8 data bits
// - No parity
// - 1 stop bit
//
// With this format the default values of the relevant bits in UCSRnB and UCSRnC
// are:
//
// -- 8 data bits --
// - UCSZn2 (in UCSRnB) = 0
// - UCSZn1 (in UCSRnC) = 1
// - UCSZn0 (in UCSRnC) = 1
//
// -- No parity --
// - UPMn1 (in UCSRnC) = 0
// - UPMn0 (in UCSRnC) = 0
//
// -- 1 stop bit --
// - USBSn (in UCSRnC) = 0
//
// Mock the current register values and bit positions for data bits to be the
// default configuration of 8 data bits, no parity, and 1 stop bit. The other
// bits in UCSRnB and UCSRnC are set to 0, but their values are not relevant for
// this test.
//==============================

TEST_F(Test_UART_AVR_fixture, SetFrameFormat_UpdatesFrameFormat_DataBits) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnB))
      .WillByDefault(testing::Return(0b0000'0000));
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnC))
      .WillByDefault(testing::Return(0b0000'0110));

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UCSZn2))
      .WillByDefault(
          testing::Return(2)); // Assume that DEF_UCSZn2 is bit 2 in UCSRnB
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UCSZn1))
      .WillByDefault(
          testing::Return(2)); // Assume that DEF_UCSZn1 is bit 2 in UCSRnC
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UCSZn0))
      .WillByDefault(
          testing::Return(1)); // Assume that DEF_UCSZn0 is bit 1 in UCSRnC

  static constexpr uint8_t expectedUCSRnBValue = 0b0000'0000;
  static constexpr uint8_t expectedUCSRnCValue = 0b0000'0010;

  UART_FrameFormat newFrameFormat = m_uart_AVR->GetFrameFormat();
  newFrameFormat.dataBits = UART_DataBits::DB6;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB,
                                            expectedUCSRnBValue))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnC,
                                            expectedUCSRnCValue))
      .Times(1);

  m_uart_AVR->SetFrameFormat(newFrameFormat);

  EXPECT_EQ(m_uart_AVR->GetFrameFormat(), newFrameFormat);
}

TEST_F(Test_UART_AVR_fixture, SetFrameFormat_UpdatesFrameFormat_Parity) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnC))
      .WillByDefault(testing::Return(0b0000'0110));

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UPMn1))
      .WillByDefault(
          testing::Return(5)); // Assume that DEF_UPMn1 is bit 5 in UCSRnC
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UPMn0))
      .WillByDefault(
          testing::Return(4)); // Assume that DEF_UPMn0 is bit 4 in UCSRnC

  static constexpr uint8_t expectedUCSRnCValue = 0b0010'0110;

  UART_FrameFormat newFrameFormat = m_uart_AVR->GetFrameFormat();
  newFrameFormat.parity = UART_Parity::EVEN;

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB, testing::_))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnC,
                                            expectedUCSRnCValue))
      .Times(1);

  m_uart_AVR->SetFrameFormat(newFrameFormat);

  EXPECT_EQ(m_uart_AVR->GetFrameFormat(), newFrameFormat);
}

TEST_F(Test_UART_AVR_fixture, SetFrameFormat_UpdatesFrameFormat_StopBits) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnC))
      .WillByDefault(testing::Return(0b0000'0110));

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_USBSn))
      .WillByDefault(
          testing::Return(3)); // Assume that DEF_USBSn is bit 3 in UCSRnC

  static constexpr uint8_t expectedUCSRnCValue = 0b0000'1110;

  UART_FrameFormat newFrameFormat = m_uart_AVR->GetFrameFormat();
  newFrameFormat.stopBits = UART_StopBits::SB2;

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB, testing::_))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnC,
                                            expectedUCSRnCValue))
      .Times(1);

  m_uart_AVR->SetFrameFormat(newFrameFormat);

  EXPECT_EQ(m_uart_AVR->GetFrameFormat(), newFrameFormat);
}

//==============================
// SetUARTMode(...)
//==============================

TEST_F(Test_UART_AVR_fixture, SetUARTMode_UpdatesUARTModeToSynchronous) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnC))
      .WillByDefault(
          testing::Return(0xF0)); // UMSELn1 and UMSELn0 are bits 7 and 6
  std::cout << "Mocked ReadRegister for REG_UCSRnC to return 0xF0" << std::endl;

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UMSELn1))
      .WillByDefault(
          testing::Return(7)); // Assume that DEF_UMSELn1 is bit 7 in UCSRnC
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UMSELn0))
      .WillByDefault(
          testing::Return(6)); // Assume that DEF_UMSELn0 is bit 6 in UCSRnC

  static constexpr uint8_t expectedUSCRnCValue = 0b0111'0000;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnC,
                                            expectedUSCRnCValue))
      .Times(1);

  m_uart_AVR->SetUARTMode(UART_Mode::SYNCHRONOUS);
  EXPECT_EQ(m_uart_AVR->GetUARTMode(), UART_Mode::SYNCHRONOUS);
}

TEST_F(Test_UART_AVR_fixture, SetUARTMode_UpdatesUARTModeToAsynchronous) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnC))
      .WillByDefault(
          testing::Return(0xF0)); // UMSELn1 and UMSELn0 are bits 7 and 6

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UMSELn1))
      .WillByDefault(
          testing::Return(7)); // Assume that DEF_UMSELn1 is bit 7 in UCSRnC
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UMSELn0))
      .WillByDefault(
          testing::Return(6)); // Assume that DEF_UMSELn0 is bit 6 in UCSRnC

  static constexpr uint8_t expectedUSCRnCValue = 0b0011'0000;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnC,
                                            expectedUSCRnCValue))
      .Times(1);

  m_uart_AVR->SetUARTMode(UART_Mode::ASYNCHRONOUS);
  EXPECT_EQ(m_uart_AVR->GetUARTMode(), UART_Mode::ASYNCHRONOUS);
}

//==============================
// Enable(...)
//==============================

TEST_F(Test_UART_AVR_fixture, Enable_SetsRXENnAndTXENnBits) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnB))
      .WillByDefault(testing::Return(0x00));

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXENn))
      .WillByDefault(
          testing::Return(4)); // Assume that DEF_RXENn is bit 4 in UCSRnB
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_TXENn))
      .WillByDefault(
          testing::Return(3)); // Assume that DEF_TXENn is bit 3 in UCSRnB

  static constexpr uint8_t expectedUCSRnBValue = 0b0001'1000;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB,
                                            expectedUCSRnBValue))
      .Times(1);

  m_uart_AVR->Enable();
}

TEST_F(Test_UART_AVR_fixture,
       Enable_SetsRXENnAndTXENnBits_withoutAffectingOtherBits) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnB))
      .WillByDefault(testing::Return(0b1000'0001));

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXENn))
      .WillByDefault(
          testing::Return(4)); // Assume that DEF_RXENn is bit 4 in UCSRnB
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_TXENn))
      .WillByDefault(
          testing::Return(3)); // Assume that DEF_TXENn is bit 3 in UCSRnB

  static constexpr uint8_t expectedUCSRnBValue = 0b1001'1001;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB,
                                            expectedUCSRnBValue))
      .Times(1);

  m_uart_AVR->Enable();
}

//==============================
// Disable(...)
//==============================

TEST_F(Test_UART_AVR_fixture, Disable_ClearsRXENnAndTXENnBits) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnB))
      .WillByDefault(testing::Return(0b0001'1000));

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXENn))
      .WillByDefault(
          testing::Return(4)); // Assume that DEF_RXENn is bit 4 in UCSRnB
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_TXENn))
      .WillByDefault(
          testing::Return(3)); // Assume that DEF_TXENn is bit 3 in UCSRnB

  static constexpr uint8_t expectedUCSRnBValue = 0b0000'0000;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB,
                                            expectedUCSRnBValue))
      .Times(1);

  m_uart_AVR->Disable();
}

TEST_F(Test_UART_AVR_fixture,
       Disable_ClearsRXENnAndTXENnBits_withoutAffectingOtherBits) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnB))
      .WillByDefault(testing::Return(0b1001'1001));

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXENn))
      .WillByDefault(
          testing::Return(4)); // Assume that DEF_RXENn is bit 4 in UCSRnB
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_TXENn))
      .WillByDefault(
          testing::Return(3)); // Assume that DEF_TXENn is bit 3 in UCSRnB

  static constexpr uint8_t expectedUCSRnBValue = 0b1000'0001;

  EXPECT_CALL(*m_mockIOProxy, WriteRegister(UART_AVR_RegisterEnum::REG_UCSRnB,
                                            expectedUCSRnBValue))
      .Times(1);

  m_uart_AVR->Disable();
}

//==============================
// SendByte(...)
//==============================

TEST_F(Test_UART_AVR_fixture, SendByte_ReadsUDREnBitBeforeSending) {
  // Set UDREn to 1 to avoid an infinite loop in SendByte() while waiting for
  // the transmit buffer to be ready.
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
      .WillByDefault(testing::Return(0b0001'0000));
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UDREn))
      .WillByDefault(
          testing::Return(4)); // Assume that DEF_UDREn is bit 4 in UCSRnA

  static constexpr uint8_t byteToSend = 0xAB;

  {
    testing::InSequence seq;

    EXPECT_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_UDREn))
        .Times(1);
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
        .Times(1);

    EXPECT_CALL(*m_mockIOProxy,
                WriteRegister(UART_AVR_RegisterEnum::REG_UDRn, byteToSend))
        .Times(1);
  }

  m_uart_AVR->SendByte(byteToSend);
}

//==============================
// ByteAvailable(...)
//==============================

TEST_F(Test_UART_AVR_fixture, ByteAvailable_ReturnsFalseWhenNoByteAvailable) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
      .WillByDefault(testing::Return(0b0000'0000));
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXCn))
      .WillByDefault(
          testing::Return(7)); // Assume that DEF_RXCn is bit 7 in UCSRnA

  {
    testing::InSequence seq;

    EXPECT_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXCn))
        .Times(1);
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
        .Times(1);
  }

  EXPECT_FALSE(m_uart_AVR->ByteAvailable());
}

TEST_F(Test_UART_AVR_fixture, ByteAvailable_ReturnsTrueWhenByteAvailable) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
      .WillByDefault(testing::Return(0b1000'0000));
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXCn))
      .WillByDefault(
          testing::Return(7)); // Assume that DEF_RXCn is bit 7 in UCSRnA

  {
    testing::InSequence seq;

    EXPECT_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXCn))
        .Times(1);
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
        .Times(1);
  }

  EXPECT_TRUE(m_uart_AVR->ByteAvailable());
}

//==============================
// ReadByte_NonBlocking(...)
//==============================

TEST_F(Test_UART_AVR_fixture,
       ReadByte_NonBlocking_ReturnsReceivedByteIfAvailable) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
      .WillByDefault(testing::Return(0b1000'0000));
  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXCn))
      .WillByDefault(
          testing::Return(7)); // Assume that DEF_RXCn is bit 7 in UCSRnA

  static constexpr uint8_t expectedByte = 0xAB;
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UDRn))
      .WillByDefault(testing::Return(expectedByte));

  {
    testing::InSequence seq;

    EXPECT_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXCn))
        .Times(1);
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
        .Times(1);
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UDRn))
        .Times(1);
  }

  EXPECT_EQ(m_uart_AVR->ReadByte_NonBlocking(), expectedByte);
}

TEST_F(Test_UART_AVR_fixture, ReadByte_NonBlocking_Returns0IfNoByteAvailable) {
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
      .WillByDefault(testing::Return(0b0000'0000));

  ON_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXCn))
      .WillByDefault(
          testing::Return(7)); // Assume that DEF_RXCn is bit 7 in UCSRnA

  static constexpr uint8_t expectedByte = 0xAB;
  ON_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UDRn))
      .WillByDefault(testing::Return(expectedByte));

  {
    testing::InSequence seq;

    EXPECT_CALL(*m_mockIOProxy, ReadDefine(UART_AVR_DefineEnum::DEF_RXCn))
        .Times(1);
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UCSRnA))
        .Times(1);
    EXPECT_CALL(*m_mockIOProxy, ReadRegister(UART_AVR_RegisterEnum::REG_UDRn))
        .Times(0);
  }

  EXPECT_EQ(m_uart_AVR->ReadByte_NonBlocking(), 0);
}