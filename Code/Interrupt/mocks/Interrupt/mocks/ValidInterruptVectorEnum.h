#pragma once

enum class VALID_INTERRUPT_VECTORS {
  IV_INTERRUPT0,
  IV_ANOTHER_INTERRUPT,

  SIZE, // This is not a valid interrupt vector, it is used to get the size of
        // the enum
};