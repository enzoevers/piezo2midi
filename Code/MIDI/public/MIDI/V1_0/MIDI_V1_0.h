#pragma once

#include "Communication/UART/IUART.h"
#include "MIDI/V1_0/IMIDI_V1_0.h"

class MIDI_V1_0 : public IMIDI_V1_0 {
public:
  MIDI_V1_0(IUART &uart);
  ~MIDI_V1_0() = default;

  //== IMIDI_V1_0 implementation ==//
  void SendStatusByte(MIDI_V1_0_STATUS status,
                      MIDI_V1_0_CHANNEL channel) override;
  auto SendDataByte(uint8_t data) -> bool override;
  //== End of IMIDI_V1_0 implementation ==//

private:
  IUART &m_uart;
  uint8_t m_lastSentStatusByte = 0; // 0 is not a valid status byte
};