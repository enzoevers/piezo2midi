#pragma once

#include "Interrupt/IAtomicOperation.h"
#include "Interrupt/IGlobalInterruptControl.h"
#include "Timing/IIOProxy_Timing_8bit_AVR.h"
#include "Timing/ITiming.h"

class Timing_8bit_AVR : public ITiming {
public:
  Timing_8bit_AVR(IIOProxy_Timing_8bit_AVR &ioProxy,
                  IAtomicOperation &atomicOperation,
                  IGlobalInterruptControl &globalInterruptControl);
  ~Timing_8bit_AVR() = default;

  //=== ITiming implementation ===//
  /**
   * @brief Configures the timer with the specified timer value in microseconds
   * for the OCRA value.
   */
  void SetupTimer(uint16_t timerValue_us) override;
  void StartTimer() override;
  void StopTimer() override;

  void Delay_us(uint32_t us) override;
  //=== End of ITiming implementation ===//

  auto GetTimerPeriod_us() const -> uint16_t;
  auto GetInterruptCount() const -> uint64_t;
  void TimerInterruptHandler();

private:
  IIOProxy_Timing_8bit_AVR &m_ioProxy;
  IAtomicOperation &m_atomicOperation;
  IGlobalInterruptControl &m_globalInterruptControl;

  uint8_t m_clockSelectMask{};
  uint16_t m_interruptPeriod_us{};

  // Even if the timer increments every microsecond, it would take about 584,942
  // years to overflow a 64-bit counter.
  volatile uint64_t m_interruptCount{};
};
