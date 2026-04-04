#pragma once

#include <stdint.h>
class ISPIMaster {
public:
#ifndef __AVR__
  virtual ~ISPIMaster() = default;
#endif

  /// @brief Selects the slave device with the given index for communication.
  /// The slave index corresponds to the position of the slave's SS pin in the
  /// SPIConfig's SS_pin array.
  /// @param slaveIndex
  /// @return True if the slave index is valid and the slave is successfully
  /// selected, false otherwise.
  virtual auto SelectSlave(uint8_t slaveIndex) -> bool = 0;

  /// @brief Delects the slave device with the given index, effectively ending
  /// communication
  /// @param slaveIndex
  /// @return  True if the slave index is valid and the slave is successfully
  /// deselected, false otherwise.
  virtual auto DeselectSlave(uint8_t slaveIndex) -> bool = 0;

  /// @brief Sends a byte to the currently selected slave and returns the byte
  /// received from the slave during the transmission.
  /// @param byteToSend
  /// @return The received byte from the slave during the transmission.
  virtual auto SendByte(const uint8_t byteToSend) -> uint8_t = 0;
};