#include <gtest/gtest.h>
#include <memory>

#include "DigitalIO/DigitalIO_AVR.h"
#include "DigitalIO/mocks/Mock_IOProxy_DigitalIO_AVR.h"

class TestDigitalIO_AVR_fixture : public ::testing::Test {
public:
  TestDigitalIO_AVR_fixture() {
    m_mockIOProxy = std::make_unique<Mock_IOProxy_DigitalIO_AVR>();
    m_digitalIO_AVR = std::make_unique<DigitalIO_AVR>(*m_mockIOProxy);

    SetDefault_ReadRegister_Mock();
  }

  void SetDefault_ReadRegister_Mock() {
    ON_CALL(*m_mockIOProxy, ReadRegister(testing::_))
        .WillByDefault(testing::Return(0b0000'0000));
  }

  std::unique_ptr<Mock_IOProxy_DigitalIO_AVR> m_mockIOProxy;
  std::unique_ptr<DigitalIO_AVR> m_digitalIO_AVR;
};

//==============================
// SetMode(...)
//==============================

TEST_F(TestDigitalIO_AVR_fixture,
       SetMode_SetsDDRRegister_WritesLogicOne_forOutput) {

  static constexpr uint8_t pin3 = 3;
  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx, 0b0000'1000))
      .Times(1);

  m_digitalIO_AVR->SetMode(pin3, PIN_MODE::OUTPUT);
}

TEST_F(TestDigitalIO_AVR_fixture,
       SetMode_SetsDDRRegister_DoesNotOverwriteOtherBits_forOutput) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx))
      .WillByDefault(testing::Return(0b0100'0001));

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx, 0b0100'1001))
      .Times(1);

  m_digitalIO_AVR->SetMode(pin3, PIN_MODE::OUTPUT);
}

TEST_F(TestDigitalIO_AVR_fixture,
       SetMode_SetsDDRRegister_WritesLogicZero_forInput) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx))
      .WillByDefault(testing::Return(0b0000'1000));

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx, 0b0000'0000))
      .Times(1);

  m_digitalIO_AVR->SetMode(pin3, PIN_MODE::INPUT);
}

TEST_F(TestDigitalIO_AVR_fixture,
       SetMode_SetsDDRRegister_DoesNotOverwriteOtherBits_forInput) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx))
      .WillByDefault(testing::Return(0b0100'1001));

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(DigitalIO_AVR_RegisterEnum::REG_DDRx, 0b0100'0001))
      .Times(1);

  m_digitalIO_AVR->SetMode(pin3, PIN_MODE::INPUT);
}

//==============================
// SetState(...)
//==============================

TEST_F(TestDigitalIO_AVR_fixture,
       SetState_SetsPORTRegister_WritesLogicOne_forHigh) {
  static constexpr uint8_t pin3 = 3;
  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx, 0b0000'1000))
      .Times(1);

  m_digitalIO_AVR->SetState(pin3, PIN_STATE::HIGH);
}

TEST_F(TestDigitalIO_AVR_fixture,
       SetState_SetsPORTRegister_DoesNotOverwriteOtherBits_forHigh) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx))
      .WillByDefault(testing::Return(0b0100'0001));

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx, 0b0100'1001))
      .Times(1);

  m_digitalIO_AVR->SetState(pin3, PIN_STATE::HIGH);
}

TEST_F(TestDigitalIO_AVR_fixture,
       SetState_SetsPORTRegister_WritesLogicZero_forLow) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx))
      .WillByDefault(testing::Return(0b0000'1000));

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx, 0b0000'0000))
      .Times(1);

  m_digitalIO_AVR->SetState(pin3, PIN_STATE::LOW);
}

TEST_F(TestDigitalIO_AVR_fixture,
       SetState_SetsPORTRegister_DoesNotOverwriteOtherBits_forLow) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx))
      .WillByDefault(testing::Return(0b0100'1001));

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(DigitalIO_AVR_RegisterEnum::REG_PORTx, 0b0100'0001))
      .Times(1);

  m_digitalIO_AVR->SetState(pin3, PIN_STATE::LOW);
};

//==============================
// GetState(...)
//==============================

TEST_F(TestDigitalIO_AVR_fixture,
       GetState_ReadsPINRegister_ReturnsHighIfBitIsOne) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_PINx))
      .WillByDefault(testing::Return(0b0000'1000));

  EXPECT_EQ(m_digitalIO_AVR->GetState(pin3), PIN_STATE::HIGH);
}

TEST_F(TestDigitalIO_AVR_fixture,
       GetState_ReadsPINRegister_ReturnsLowIfBitIsZero) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_PINx))
      .WillByDefault(testing::Return(0b0000'0000));

  EXPECT_EQ(m_digitalIO_AVR->GetState(pin3), PIN_STATE::LOW);
}

TEST_F(TestDigitalIO_AVR_fixture,
       GetState_ReadsPINRegister_ReturnsLowIfBitIsZero_RegardlessOfOtherBits) {
  static constexpr uint8_t pin3 = 3;
  ON_CALL(*m_mockIOProxy, ReadRegister(DigitalIO_AVR_RegisterEnum::REG_PINx))
      .WillByDefault(testing::Return(0b1111'0111));

  EXPECT_EQ(m_digitalIO_AVR->GetState(pin3), PIN_STATE::LOW);
}
