#include "ADC/MCP3008/MCP3008.h"

MCP3008::MCP3008(ISPIMaster &spiMaster) : m_spiMaster(spiMaster) {}

auto MCP3008::AdcResolution() -> uint16_t {
  return 1024; // 10-bit ADC has a resolution of 1024 discrete values (0-1023)
}

auto MCP3008::ReadChannel(uint8_t channel) -> uint16_t {
  if (channel > 7) {
    return 0; // Invalid channel, return 0 or handle as desired
  }

  m_spiMaster.SendByte(0b0000'0001); // Start bit
  // For single-ended mode, the second byte has the format: 1 (single-ended) |
  // channel (3 bits) | 4 don't care bits
  uint8_t singleEndedCommand = 0b1000'0000 | (channel << 4);
  uint16_t readAdcValue =
      (m_spiMaster.SendByte(singleEndedCommand) & 0b11)
      << 8; // Send second byte and shift the response to the upper byte
  readAdcValue |= m_spiMaster.SendByte(
      0); // Send third byte and combine with the previous response

  return readAdcValue;
}