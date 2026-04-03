#pragma once

#include "Communication/SPI/IIOProxy_SPI_AVR.h"
#include "Communication/SPI/ISPIMaster.h"
#include "Communication/SPI/SPIPinConfig.h"
#include "Communication/SPI/SPI_DATAORDER.h"
#include "Communication/SPI/SPI_MODE.h"
#include "DigitalIO/IDigitalIO.h"

enum class ROUND_DIRECTION { UP, DOWN, CLOSEST };

class SPIMaster_AVR : public ISPIMaster {
public:
  SPIMaster_AVR(IIOProxy_SPI_AVR &ioProxy, SPIPinConfig pinConfig,
                SPI_MODE mode = SPI_MODE::MODE0, uint32_t clk_hz = 1000000,
                SPI_DATAORDER dataOrder = SPI_DATAORDER::MSB_FIRST);
  ~SPIMaster_AVR() = default;

  //=== ISPIMaster implementation ===//
  auto SelectSlave(uint8_t slaveIndex) -> bool override;
  auto SendByte(const uint8_t byteToSend) -> uint8_t override;
  //=== End of ISPIMaster implementation ===//

  void SetMode(SPI_MODE mode);
  void
  SetClockFrequency(uint32_t clk_hz,
                    ROUND_DIRECTION roundDirection = ROUND_DIRECTION::DOWN);
  void SetDataOrder(SPI_DATAORDER dataOrder);

  auto GetMode() const -> SPI_MODE;
  auto GetClockFrequency() const -> uint32_t;
  auto GetDataOrder() const -> SPI_DATAORDER;

  void SetPinModes();
  void SetMasterMode();
  void EnableSPI();
  void DisableSPI();

private:
  IIOProxy_SPI_AVR &m_ioProxy;
  SPIPinConfig m_pinConfig;
};
