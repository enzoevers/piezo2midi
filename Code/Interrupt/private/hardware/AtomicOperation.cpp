#include "Interrupt/AtomicOperation.h"
#include <util/atomic.h>

void AtomicOperation::ExecuteAtomicOperation(TypeErasedCallable &operation) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { operation(); }
}