#include <gtest/gtest.h>
#include <memory>

#include "Communication/UART/mocks/Mock_IUART.h"
#include "MIDI/V1_0/MIDI_V1_0.h"

class Test_MIDI_V1_0_Fixture : public ::testing::Test {
public:
  Test_MIDI_V1_0_Fixture()
      : m_uart(std::make_unique<Mock_IUART>()),
        m_midi(std::make_unique<MIDI_V1_0>(*m_uart)) {}

  std::unique_ptr<Mock_IUART> m_uart;
  std::unique_ptr<MIDI_V1_0> m_midi;
};

//==============================
// SendStatusByte(...)
//==============================

TEST_F(Test_MIDI_V1_0_Fixture, SendStatusByte_SendsCorrectStatusByte_NoteOff) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::NOTE_OFF;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByte =
      0x84; // 0x80 (NOTE_OFF) + 0x04 (channel 5)

  EXPECT_CALL(*m_uart, SendByte(expectedStatusByte)).Times(1);

  m_midi->SendStatusByte(status, channel);
}

TEST_F(Test_MIDI_V1_0_Fixture, SendStatusByte_SendsCorrectStatusByte_NoteOn) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::NOTE_ON;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByte =
      0x94; // 0x90 (NOTE_ON) + 0x04 (channel 5)

  EXPECT_CALL(*m_uart, SendByte(expectedStatusByte)).Times(1);

  m_midi->SendStatusByte(status, channel);
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendStatusByte_SendsCorrectStatusByte_PolyphonicKeyPressure) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::POLYPHONIC_KEY_PRESSURE;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByte =
      0xA4; // 0xA0 (POLYPHONIC_KEY_PRESSURE) + 0x04 (channel 5)

  EXPECT_CALL(*m_uart, SendByte(expectedStatusByte)).Times(1);

  m_midi->SendStatusByte(status, channel);
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendStatusByte_SendsCorrectStatusByte_ControlChange) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::CONTROL_CHANGE;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByte =
      0xB4; // 0xB0 (CONTROL_CHANGE) + 0x04 (channel 5)

  EXPECT_CALL(*m_uart, SendByte(expectedStatusByte)).Times(1);

  m_midi->SendStatusByte(status, channel);
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendStatusByte_SendsCorrectStatusByte_ProgramChange) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::PROGRAM_CHANGE;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByte =
      0xC4; // 0xC0 (PROGRAM_CHANGE) + 0x04 (channel 5)

  EXPECT_CALL(*m_uart, SendByte(expectedStatusByte)).Times(1);

  m_midi->SendStatusByte(status, channel);
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendStatusByte_SendsCorrectStatusByte_ChannelPressure) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::CHANNEL_PRESSURE;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByte =
      0xD4; // 0xD0 (CHANNEL_PRESSURE) + 0x04 (channel 5)

  EXPECT_CALL(*m_uart, SendByte(expectedStatusByte)).Times(1);

  m_midi->SendStatusByte(status, channel);
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendStatusByte_SendsCorrectStatusByte_PitchBendChange) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::PITCH_BEND_CHANGE;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByte =
      0xE4; // 0xE0 (PITCH_BEND_CHANGE) + 0x04 (channel 5)

  EXPECT_CALL(*m_uart, SendByte(expectedStatusByte)).Times(1);

  m_midi->SendStatusByte(status, channel);
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendStatusByte_DoesNotSendSameStatusByteTwice_RunningStatus) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::NOTE_ON;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByte =
      0x94; // 0x90 (NOTE_ON) + 0x04 (channel 5)

  EXPECT_CALL(*m_uart, SendByte(expectedStatusByte)).Times(1);

  m_midi->SendStatusByte(status, channel);
  m_midi->SendStatusByte(status, channel);
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendStatusByte_SendNewStatusByteIfOnlyChannelChanges_RunningStatus) {
  MIDI_V1_0_STATUS status = MIDI_V1_0_STATUS::NOTE_ON;
  MIDI_V1_0_CHANNEL channel_firstCall = MIDI_V1_0_CHANNEL::CH5;
  MIDI_V1_0_CHANNEL channel_secondCall = MIDI_V1_0_CHANNEL::CH2;

  static constexpr uint8_t expectedStatusByteFirstCall =
      0x94; // 0x90 (NOTE_ON) + 0x04 (channel 5)
  static constexpr uint8_t expectedStatusByteSecondCall =
      0x91; // 0x90 (NOTE_ON) + 0x01 (channel 2)

  {

    testing::InSequence seq;

    EXPECT_CALL(*m_uart, SendByte(expectedStatusByteFirstCall)).Times(1);
    EXPECT_CALL(*m_uart, SendByte(expectedStatusByteSecondCall)).Times(1);
  }

  m_midi->SendStatusByte(status, channel_firstCall);
  m_midi->SendStatusByte(status, channel_secondCall);
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendStatusByte_SendNewStatusByteIfOnlyStatusChanges_RunningStatus) {
  MIDI_V1_0_STATUS status_firstCall = MIDI_V1_0_STATUS::NOTE_ON;
  MIDI_V1_0_STATUS status_secondCall = MIDI_V1_0_STATUS::NOTE_OFF;
  MIDI_V1_0_CHANNEL channel = MIDI_V1_0_CHANNEL::CH5;

  static constexpr uint8_t expectedStatusByteFirstCall =
      0x94; // 0x90 (NOTE_ON) + 0x04 (channel 5)
  static constexpr uint8_t expectedStatusByteSecondCall =
      0x84; // 0x80 (NOTE_OFF) + 0x04 (channel 5)

  {

    testing::InSequence seq;

    EXPECT_CALL(*m_uart, SendByte(expectedStatusByteFirstCall)).Times(1);
    EXPECT_CALL(*m_uart, SendByte(expectedStatusByteSecondCall)).Times(1);
  }

  m_midi->SendStatusByte(status_firstCall, channel);
  m_midi->SendStatusByte(status_secondCall, channel);
}

// System common messages (0xF0 to 0xF7) and system real-time messages (0xF8 to
// 0xFF) are not implemented at this moment.

//==============================
// SendDataByte(...)
//==============================

TEST_F(Test_MIDI_V1_0_Fixture,
       SendDataByte_ReturnTrueAndSendsBytesLessThan128) {
  for (uint16_t data = 0; data < 128; data += 10) {
    EXPECT_CALL(*m_uart, SendByte(static_cast<uint8_t>(data))).Times(1);
    EXPECT_TRUE(m_midi->SendDataByte(static_cast<uint8_t>(data)));
  }
}

TEST_F(Test_MIDI_V1_0_Fixture,
       SendDataByte_ReturnFalseAndDoesNotSendBytesLargerThan127) {
  for (uint16_t data = 128; data <= 255; data += 10) {
    EXPECT_CALL(*m_uart, SendByte(testing::_)).Times(0);
    EXPECT_FALSE(m_midi->SendDataByte(static_cast<uint8_t>(data)));
  }
}
