#include "MIDI/V1_0/MIDI_V1_0.h"

MIDI_V1_0::MIDI_V1_0(IUART &uart) : m_uart(uart) {}

void MIDI_V1_0::SendStatusByte(MIDI_V1_0_STATUS status,
                               MIDI_V1_0_CHANNEL channel) {
  const uint8_t statusByte =
      static_cast<uint8_t>(status) + static_cast<uint8_t>(channel);

  if (statusByte != m_lastSentStatusByte) {
    m_lastSentStatusByte = statusByte;
  } else {
    // If the same status byte is sent again, it is a running status and we
    // should not send the status byte again.
    return;
  }

  m_uart.SendByte(statusByte);
}

auto MIDI_V1_0::SendDataByte(uint8_t data) -> bool {
  // The MSB of a data byte must be 0.
  if (data & 0x80) {
    return false;
  }

  m_uart.SendByte(data);
  return true;
}