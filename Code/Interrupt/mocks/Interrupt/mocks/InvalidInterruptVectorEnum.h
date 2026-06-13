#pragma once

enum class INVALID_INTERRUPT_VECTORS {
  IV_INTERRUPT0,
  IV_ANOTHER_INTERRUPT,

  // Missing the SIZE enumerator on purpose to test that the code correctly
  // handles invalid interrupt vector enums.
};