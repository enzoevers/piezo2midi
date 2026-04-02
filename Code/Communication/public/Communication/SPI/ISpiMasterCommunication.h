#pragma once

#include <stddef.h>

class ISpiMasterCommunication {
public:
  // virtual ~ISpiMasterCommunication() = default; // AVR has an issue not not
  // having libstdc++ linked in, so we can't have a virtual destructor.

  virtual void SendBytes(const size_t size, const char *const bytesToSend,
                         char *const responseBuffer) = 0;
};