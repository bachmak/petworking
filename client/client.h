#pragma once

#include "forwards.h"

namespace ese {

class Client {
 public:
  virtual void Start() = 0;
  virtual ~Client() = default;
};
}  // namespace ese