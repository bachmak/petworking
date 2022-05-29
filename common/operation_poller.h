#pragma once

#include "forwards.h"

namespace ese {

class OperationPoller {
 public:
  explicit OperationPoller() = default;

 public:
  void StartPolling() { context_.run(); }

  void StopPolling() { context_.stop(); }

 protected:
  Context context_;
};
}  // namespace ese