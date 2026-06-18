#pragma once

#include "ChannelMapping/IVelocityMap.h"
#include "PiezoPeakDetector/IPiezoPeakDetector.h"

struct MidiChannel {
  uint8_t channelNumber{};
  uint8_t midiChannel{};
  IPiezoPeakDetector *piezoPeakDetector{};
  IVelocityMap *velocityMap{};

  bool operator==(const MidiChannel &other) const {
    return channelNumber == other.channelNumber &&
           midiChannel == other.midiChannel &&
           piezoPeakDetector == other.piezoPeakDetector &&
           velocityMap == other.velocityMap;
  }
};
