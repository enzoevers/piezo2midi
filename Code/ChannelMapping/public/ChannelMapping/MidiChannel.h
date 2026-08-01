#pragma once

#include "ChannelMapping/IVelocityMap.h"
#include "MIDI/V1_0/MIDI_V1_0_CHANNEL.h"
#include "PiezoPeakDetector/IPiezoPeakDetector.h"

/**
 * @brief Represents a MIDI channel with associated piezo peak detector and
 * velocity map. This struct does not manage any lifetime of objects.
 */
struct MidiChannel {
  uint8_t adcChannel{};
  MIDI_V1_0_CHANNEL midiChannel{};
  IPiezoPeakDetector *piezoPeakDetector{};
  IVelocityMap *velocityMap{};

  bool operator==(const MidiChannel &other) const {
    return adcChannel == other.adcChannel && midiChannel == other.midiChannel &&
           piezoPeakDetector == other.piezoPeakDetector &&
           velocityMap == other.velocityMap;
  }
};
