#pragma once

#include "forwards.h"

namespace ese {

class Server {
 public:
  virtual ~Server() = default;

 public:
  virtual void Start(ServerCallback onPacketReceived) = 0;
};
}  // namespace ese