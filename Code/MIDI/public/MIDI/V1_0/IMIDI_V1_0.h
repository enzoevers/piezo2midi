#pragma once

#include "MIDI/V1_0/MIDI_V1_0_CHANNEL.h"
#include "MIDI/V1_0/MIDI_V1_0_STATUS.h"

class IMIDI_V1_0 {
public:
#ifndef __AVR__
  virtual ~IMIDI_V1_0() = default;
#endif

  virtual void SendStatusByte(MIDI_V1_0_STATUS status,
                              MIDI_V1_0_CHANNEL channel) = 0;
  virtual auto SendDataByte(uint8_t data) -> bool = 0;
};