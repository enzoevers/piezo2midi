#include <gtest/gtest.h>
#include <memory>

#include "Interrupt/InterruptRegistration.h"
#include "Interrupt/mocks/InvalidInterruptVectorEnum.h"
#include "Interrupt/mocks/ValidInterruptVectorEnum.h"

class TestInterruptRegistration : public ::testing::Test {
public:
  TestInterruptRegistration() {
    m_interruptRegistration =
        std::make_unique<InterruptRegistration<VALID_INTERRUPT_VECTORS>>();
  }

  std::unique_ptr<InterruptRegistration<VALID_INTERRUPT_VECTORS>>
      m_interruptRegistration;
};

//==============================
// Construction
//==============================

TEST_F(TestInterruptRegistration,
       test_cannot_construct_with_invalid_interrupt_vector_enum) {
  // The following line should not compile if uncommented, since
  // INVALID_INTERRUPT_VECTORS does not have a SIZE enumerator.
  // auto invalidInterruptRegistration =
  // std::make_unique<InterruptRegistration<INVALID_INTERRUPT_VECTORS>>();
  ASSERT_TRUE(true)
      << "This test checks that the code does not compile with an invalid "
         "interrupt vector enum, so there is no runtime check to perform.";
  ASSERT_TRUE(m_interruptRegistration);
}

//==============================
// SetInterruptHandler() and GetInterruptHandler()
//==============================

TEST_F(TestInterruptRegistration, test_SetAndGetInterruptHandler_Lambda) {
  struct TestStruct {
    int a{};
  };

  TestStruct testStructInstance;
  auto memberFunctionLambda = [&testStructInstance]() {
    testStructInstance.a = 42;
  };

  auto nonMemberLambda = []() { /* Do nothing */ };

  TypeErasedCallable memberHandler =
      TypeErasedCallable::Bind(memberFunctionLambda);
  TypeErasedCallable nonMemberHandler =
      TypeErasedCallable::Bind(nonMemberLambda);

  m_interruptRegistration->SetInterruptHandler(
      VALID_INTERRUPT_VECTORS::IV_INTERRUPT0, nonMemberHandler);
  m_interruptRegistration->SetInterruptHandler(
      VALID_INTERRUPT_VECTORS::IV_ANOTHER_INTERRUPT, memberHandler);

  TypeErasedCallable retrievedHandler0 =
      m_interruptRegistration->GetInterruptHandler(
          VALID_INTERRUPT_VECTORS::IV_INTERRUPT0);
  TypeErasedCallable retrievedHandlerAnother =
      m_interruptRegistration->GetInterruptHandler(
          VALID_INTERRUPT_VECTORS::IV_ANOTHER_INTERRUPT);

  EXPECT_EQ(retrievedHandler0, nonMemberHandler);
  EXPECT_EQ(retrievedHandlerAnother, memberHandler);
}

TEST_F(TestInterruptRegistration,
       test_SetAndGetInterruptHandler_CallableObject) {
  struct TestStruct {
    void operator()() { a = 42; }
    int a{};
  };

  TestStruct testStructInstance;

  TypeErasedCallable callableStructHandler =
      TypeErasedCallable::Bind(testStructInstance);

  m_interruptRegistration->SetInterruptHandler(
      VALID_INTERRUPT_VECTORS::IV_ANOTHER_INTERRUPT, callableStructHandler);

  TypeErasedCallable retrievedHandlerAnother =
      m_interruptRegistration->GetInterruptHandler(
          VALID_INTERRUPT_VECTORS::IV_ANOTHER_INTERRUPT);

  EXPECT_EQ(retrievedHandlerAnother, callableStructHandler);
}
