#pragma once

#include "ChannelMapping/IVelocityMap.h"
#include <gmock/gmock.h>

class Mock_IVelocityMap : public IVelocityMap {
public:
  MOCK_METHOD(uint16_t, MapVelocity, (uint16_t velocity), (const, override));
};