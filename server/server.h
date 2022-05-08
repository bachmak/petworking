#pragma once

#include "forwards.h"

namespace ese {

class Server {
 public:
  virtual void Start() = 0;
  virtual ~Server() = default;
};
}  // namespace ese