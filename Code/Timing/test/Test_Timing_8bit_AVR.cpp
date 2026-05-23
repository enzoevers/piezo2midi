#include <gtest/gtest.h>
#include <memory>

#include "Interrupt/mocks/Mock_IAtomicOperation.h"
#include "Interrupt/mocks/Mock_IGlobalInterruptControl.h"
#include "Timing/Timing_8bit_AVR.h"
#include "Timing/mocks/Mock_IOProxy_Timing_8bit_AVR.h"

class TestTiming_8bit_AVR_fixture : public ::testing::Test {
public:
  TestTiming_8bit_AVR_fixture() {
    m_mockIOProxy = std::make_unique<Mock_IOProxy_Timing_8bit_AVR>();
    m_mockAtomicOperation = std::make_unique<Mock_IAtomicOperation>();
    m_mockGlobalInterruptControl =
        std::make_unique<Mock_IGlobalInterruptControl>();
    m_timing_8bit_AVR = std::make_unique<Timing_8bit_AVR>(
        *m_mockIOProxy, *m_mockAtomicOperation, *m_mockGlobalInterruptControl);

    SetDefault_ReadRegister_Mock();
    SetupReadDefineMocks();
  }

  void SetDefault_ReadRegister_Mock() {
    ON_CALL(*m_mockIOProxy, ReadRegister(testing::_))
        .WillByDefault(testing::Return(0x00));
  }

  void SetupReadDefineMocks() {
    // TCCRxA
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_COMxA1))
        .WillByDefault(testing::Return(7));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_COMxA0))
        .WillByDefault(testing::Return(6));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_COMxB1))
        .WillByDefault(testing::Return(5));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_COMxB0))
        .WillByDefault(testing::Return(4));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_WGMx1))
        .WillByDefault(testing::Return(1));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_WGMx0))
        .WillByDefault(testing::Return(0));

    // TCCRxB
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_FOCxA))
        .WillByDefault(testing::Return(7));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_FOCxB))
        .WillByDefault(testing::Return(6));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_WGMx2))
        .WillByDefault(testing::Return(3));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx2))
        .WillByDefault(testing::Return(2));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx1))
        .WillByDefault(testing::Return(1));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx0))
        .WillByDefault(testing::Return(0));

    // TIMSKx
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_OCIExB))
        .WillByDefault(testing::Return(2));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_OCIExA))
        .WillByDefault(testing::Return(1));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_TOIEx))
        .WillByDefault(testing::Return(0));

    // TIFRx
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_OCFxB))
        .WillByDefault(testing::Return(2));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_OCFxA))
        .WillByDefault(testing::Return(1));
    ON_CALL(*m_mockIOProxy, ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_TOVx))
        .WillByDefault(testing::Return(0));
  }

  std::unique_ptr<Mock_IOProxy_Timing_8bit_AVR> m_mockIOProxy;
  std::unique_ptr<Mock_IAtomicOperation> m_mockAtomicOperation;
  std::unique_ptr<Mock_IGlobalInterruptControl> m_mockGlobalInterruptControl;
  std::unique_ptr<Timing_8bit_AVR> m_timing_8bit_AVR;
};

//==============================
// SetupTimer(...)
//
// Assuming an F_CPU of 16 MHz
//==============================

TEST_F(TestTiming_8bit_AVR_fixture,
       test_SetupTimer_SetsCorrectRegistersFor1ms) {

  static constexpr uint8_t expectedTCCRAValue = 0b0000'0010;
  static constexpr uint8_t expectedTCCRBValue = 0b0000'0000;
  static constexpr uint8_t expectedTCNTValue = 0b0000'0000;
  static constexpr uint8_t expectedOCRAValue =
      0b1111'1001; // 249 since the timer counts from 0 to OCRxA, which gives
                   // 250 counts total
  static constexpr uint8_t expectedIFRValue = 0b0000'0010;
  static constexpr uint8_t expectedTIMSKValue = 0b0000'0010;

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxA,
                            expectedTCCRAValue))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB,
                            expectedTCCRBValue))
      .Times(1);
  EXPECT_CALL(
      *m_mockIOProxy,
      WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_OCRxA, expectedOCRAValue))
      .Times(1);
  EXPECT_CALL(
      *m_mockIOProxy,
      WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCNTx, expectedTCNTValue))
      .Times(1);
  EXPECT_CALL(
      *m_mockIOProxy,
      WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TIFRx, expectedIFRValue))
      .Times(1);
  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TIMSKx,
                            expectedTIMSKValue))
      .Times(1);

  static constexpr uint16_t oneMillisecond_us = 1000;
  m_timing_8bit_AVR->SetupTimer(oneMillisecond_us);
}

//==============================
// StartTimer()
//
// Assuming an F_CPU of 16 MHz
//==============================

TEST_F(TestTiming_8bit_AVR_fixture, test_StartTimer_SetsCorrectPrescaler) {
  static constexpr uint16_t oneMillisecond_us = 1000;
  m_timing_8bit_AVR->SetupTimer(oneMillisecond_us);

  static constexpr uint8_t expectedTCCRBValue = 0b0000'0011; // 64 prescaler

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB,
                            expectedTCCRBValue))
      .Times(1);

  m_timing_8bit_AVR->StartTimer();
}

//==============================
// StopTimer()
//==============================

TEST_F(TestTiming_8bit_AVR_fixture, test_StopTimer_ClearsPrescalerBits) {
  static constexpr uint8_t expectedTCCRBValue = 0b0000'0000; // No clock source

  EXPECT_CALL(*m_mockIOProxy,
              WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB,
                            expectedTCCRBValue))
      .Times(1);

  m_timing_8bit_AVR->StopTimer();
}

//==============================
// GetTimerPeriod_us()
//
// Assuming an F_CPU of 16 MHz
//==============================

TEST_F(TestTiming_8bit_AVR_fixture,
       test_GetTimerPeriod_us_ReturnsCorrectValue) {
  static constexpr uint16_t oneMillisecond_us = 1000;
  m_timing_8bit_AVR->SetupTimer(oneMillisecond_us);

  const uint16_t timerPeriod = m_timing_8bit_AVR->GetTimerPeriod_us();

  // Note that this is only exactly 1 ms due to the assumption of a 16 MHz clock
  // and the chosen prescaler.
  EXPECT_EQ(timerPeriod, oneMillisecond_us);
}

//==============================
// Delay_us()
//==============================

TEST_F(TestTiming_8bit_AVR_fixture, test_Delay_us_checksIfInterruptsEnabled) {
  static constexpr uint16_t oneMillisecond_us = 1000;
  m_timing_8bit_AVR->SetupTimer(oneMillisecond_us);

  EXPECT_CALL(*m_mockGlobalInterruptControl, GlobalInterruptsEnabled())
      .Times(1)
      .WillOnce(testing::Return(false));

  m_timing_8bit_AVR->Delay_us(1'000'000);
}

//==============================
// TimerInterruptHandler()
//==============================

TEST_F(TestTiming_8bit_AVR_fixture,
       test_TimerInterruptHandler_IncrementsInterruptCount) {
  const uint64_t initialCount = m_timing_8bit_AVR->GetInterruptCount();

  m_timing_8bit_AVR->TimerInterruptHandler();
  uint64_t newCount = m_timing_8bit_AVR->GetInterruptCount();
  EXPECT_EQ(newCount, initialCount + 1);

  m_timing_8bit_AVR->TimerInterruptHandler();
  newCount = m_timing_8bit_AVR->GetInterruptCount();
  EXPECT_EQ(newCount, initialCount + 2);
}

TEST_F(TestTiming_8bit_AVR_fixture,
       test_TimerInterruptHandler_UsesAtomicOperation) {
  EXPECT_CALL(*m_mockAtomicOperation, ExecuteAtomicOperation(testing::_))
      .Times(testing::AtLeast(1));

  m_timing_8bit_AVR->TimerInterruptHandler();
}
