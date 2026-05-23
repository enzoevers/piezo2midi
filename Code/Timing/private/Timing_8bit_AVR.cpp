#include "Timing/Timing_8bit_AVR.h"

namespace {

static auto BitMaskFromBitPosition(uint8_t bitPosition) -> uint8_t {
  if (bitPosition >= 8U) {
    return 0;
  }

  return static_cast<uint8_t>(1U << bitPosition);
}

struct ClockConfig {
  uint16_t prescaler;
  uint8_t csMask;
};

} // namespace

Timing_8bit_AVR::Timing_8bit_AVR(
    IIOProxy_Timing_8bit_AVR &ioProxy, IAtomicOperation &atomicOperation,
    IGlobalInterruptControl &globalInterruptControl)
    : m_ioProxy(ioProxy), m_atomicOperation(atomicOperation),
      m_globalInterruptControl(globalInterruptControl) {}

// For an 8-bit timer, the maximum compare value is 255, which corresponds to a
// maximum timer duration of (prescaler * 256) / F_CPU seconds.
void Timing_8bit_AVR::SetupTimer(uint16_t timerValue_us) {
  static constexpr uint64_t oneSecondInMicroseconds = 1'000'000UL;
  static constexpr uint64_t maxCompareValue = 0xFFU;
  static constexpr uint64_t cpuFrequencyHz = static_cast<uint64_t>(F_CPU);

  const auto wgm2Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_WGMx2);
  const auto wgm1Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_WGMx1);
  const auto wgm0Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_WGMx0);
  const auto cs2Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx2);
  const auto cs1Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx1);
  const auto cs0Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx0);
  const auto ocieaBit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_OCIExA);
  const auto ocfaBit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_OCFxA);
  //   const auto coma0Bit =
  //       m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_COMxA0);

  const auto wgm2Mask = BitMaskFromBitPosition(wgm2Bit);
  const auto wgm1Mask = BitMaskFromBitPosition(wgm1Bit);
  const auto wgm0Mask = BitMaskFromBitPosition(wgm0Bit);
  const auto cs2Mask = BitMaskFromBitPosition(cs2Bit);
  const auto cs1Mask = BitMaskFromBitPosition(cs1Bit);
  const auto cs0Mask = BitMaskFromBitPosition(cs0Bit);
  const auto ocieaMask = BitMaskFromBitPosition(ocieaBit);
  const auto ocfaMask = BitMaskFromBitPosition(ocfaBit);
  //   const auto coma0Mask = BitMaskFromBitPosition(coma0Bit);
  const auto csAllMask = static_cast<uint8_t>(cs2Mask | cs1Mask | cs0Mask);

  uint8_t tccraValue =
      m_ioProxy.ReadRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxA);
  uint8_t tccrbValue =
      m_ioProxy.ReadRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB);

  // Configure CTC mode (WGMx2:0 = 0b010).
  tccraValue = static_cast<uint8_t>(tccraValue & ~wgm0Mask);
  tccraValue = static_cast<uint8_t>(tccraValue | wgm1Mask);
  tccrbValue = static_cast<uint8_t>(tccrbValue & ~wgm2Mask);

  // Keep timer stopped until StartTimer() explicitly enables the clock source.
  tccrbValue = static_cast<uint8_t>(tccrbValue & ~csAllMask);

  const ClockConfig clockConfigs[] = {
      {1U, cs0Mask},
      {8U, cs1Mask},
      {64U, static_cast<uint8_t>(cs1Mask | cs0Mask)},
      {256U, cs2Mask},
      {1024U, static_cast<uint8_t>(cs2Mask | cs0Mask)},
  };

  const uint64_t clampedUs = (timerValue_us == 0U) ? 1U : timerValue_us;
  uint64_t compareValue = maxCompareValue;
  uint64_t prescaler = clockConfigs[4].prescaler;
  m_clockSelectMask = clockConfigs[4].csMask;

  for (const auto &clockConfig : clockConfigs) {
    const uint64_t ticks = (static_cast<uint64_t>(cpuFrequencyHz) * clampedUs) /
                           (static_cast<uint64_t>(clockConfig.prescaler) *
                            oneSecondInMicroseconds);

    if (ticks == 0U) {
      continue;
    }

    const uint64_t candidateCompare = ticks - 1U;
    if (candidateCompare <= maxCompareValue) {
      compareValue = candidateCompare;
      prescaler = clockConfig.prescaler;
      m_clockSelectMask = clockConfig.csMask;
      break;
    }
  }

  m_interruptPeriod_us = static_cast<uint16_t>(
      ((compareValue + 1U) * static_cast<uint64_t>(prescaler) *
       oneSecondInMicroseconds) /
      cpuFrequencyHz);

  uint8_t timskValue =
      m_ioProxy.ReadRegister(Timing_8bit_AVR_RegisterEnum::REG_TIMSKx);
  timskValue = static_cast<uint8_t>(timskValue | ocieaMask);

  //   tccraValue = static_cast<uint8_t>(tccraValue | coma0Mask);

  m_ioProxy.WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxA, tccraValue);
  m_ioProxy.WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB, tccrbValue);
  m_ioProxy.WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_OCRxA,
                          static_cast<uint8_t>(compareValue));
  m_ioProxy.WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCNTx, 0U);
  m_ioProxy.WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TIFRx, ocfaMask);
  m_ioProxy.WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TIMSKx, timskValue);
}

void Timing_8bit_AVR::StartTimer() {
  const auto cs2Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx2);
  const auto cs1Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx1);
  const auto cs0Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx0);
  const auto cs2Mask = BitMaskFromBitPosition(cs2Bit);
  const auto cs1Mask = BitMaskFromBitPosition(cs1Bit);
  const auto cs0Mask = BitMaskFromBitPosition(cs0Bit);
  const auto csAllMask = static_cast<uint8_t>(cs2Mask | cs1Mask | cs0Mask);

  uint8_t tccrbValue =
      m_ioProxy.ReadRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB);
  tccrbValue = static_cast<uint8_t>(tccrbValue & ~csAllMask);
  tccrbValue = static_cast<uint8_t>(tccrbValue | m_clockSelectMask);

  m_ioProxy.WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB, tccrbValue);
}

void Timing_8bit_AVR::StopTimer() {
  const auto cs2Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx2);
  const auto cs1Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx1);
  const auto cs0Bit =
      m_ioProxy.ReadDefine(Timing_8bit_AVR_DefineEnum::DEF_CSx0);
  const auto cs2Mask = BitMaskFromBitPosition(cs2Bit);
  const auto cs1Mask = BitMaskFromBitPosition(cs1Bit);
  const auto cs0Mask = BitMaskFromBitPosition(cs0Bit);
  const auto csAllMask = static_cast<uint8_t>(cs2Mask | cs1Mask | cs0Mask);

  uint8_t tccrbValue =
      m_ioProxy.ReadRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB);
  tccrbValue = static_cast<uint8_t>(tccrbValue & ~csAllMask);

  m_ioProxy.WriteRegister(Timing_8bit_AVR_RegisterEnum::REG_TCCRxB, tccrbValue);
}

void Timing_8bit_AVR::Delay_us(uint32_t us) {
  if (m_interruptPeriod_us == 0U || us == 0U) {
    return;
  }

  if (!m_globalInterruptControl.GlobalInterruptsEnabled()) {
    // If interrupts are not enabled, this function will run forever.
    return;
  }

  uint64_t currentInterruptCount{};

  auto readInterruptCount = [this, &currentInterruptCount]() {
    currentInterruptCount = m_interruptCount;
  };
  auto operation = TypeErasedCallable::Bind(readInterruptCount);

  m_atomicOperation.ExecuteAtomicOperation(operation);
  const uint64_t startCount = currentInterruptCount;

  const uint64_t interruptCountsNeeded =
      (static_cast<uint64_t>(us) + m_interruptPeriod_us - 1) /
      m_interruptPeriod_us; // Round up

  while (true) {
    m_atomicOperation.ExecuteAtomicOperation(operation);
    const uint64_t elapsed = currentInterruptCount - startCount;

    if (elapsed >= interruptCountsNeeded) {
      break;
    }
  }
}

auto Timing_8bit_AVR::GetTimerPeriod_us() const -> uint16_t {
  return m_interruptPeriod_us;
}

auto Timing_8bit_AVR::GetInterruptCount() const -> uint64_t {
  uint64_t currentInterruptCount{};
  auto readInterruptCount = [this, &currentInterruptCount]() {
    currentInterruptCount = m_interruptCount;
  };
  auto operation = TypeErasedCallable::Bind(readInterruptCount);
  m_atomicOperation.ExecuteAtomicOperation(operation);

  return currentInterruptCount;
}

void Timing_8bit_AVR::TimerInterruptHandler() {
  // TODO: Check performance
  auto incrementInterruptCount = [this]() {
    uint64_t tmp = m_interruptCount;
    ++tmp;
    m_interruptCount = tmp;
  };

  auto operation = TypeErasedCallable::Bind(incrementInterruptCount);
  m_atomicOperation.ExecuteAtomicOperation(operation);
}
