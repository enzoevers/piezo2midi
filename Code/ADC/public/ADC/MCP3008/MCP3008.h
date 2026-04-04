#pragma once

#include "ADC/IAdcSampler.h"
#include "Communication/SPI/ISPIMaster.h"
#include <stdint.h>

class MCP3008 : public IAdcSampler<uint16_t> {
public:
  MCP3008(ISPIMaster &spiMaster);
  ~MCP3008() = default;

  //== IAdcSampler implementation ===//
  auto AdcResolution() -> uint16_t override;

  /// @brief For the MCP3008, the channel parameter should be in the
  ///        range [0, 7] since it has 8 channels.
  ///        The channel numer will be read in single mode.
  ///        Before calling this method, the caller should pull the SS pin low
  ///        to select the MCP3008 and after calling this method, the caller
  ///        should pull the SS pin high to deselect the MCP3008.
  /// @param channel
  /// @return The read value in a range of [0, 1023].
  auto ReadChannel(uint8_t channel) -> uint16_t override;
  //== End of IAdcSampler implementation ==//

private:
  ISPIMaster &m_spiMaster;
};