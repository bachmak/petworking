#pragma once

#include "forwards.h"

namespace ese {

class Client {
 public:
  virtual ~Client() = default;

 public:
  virtual void Start(VoidCallback onStarted) = 0;

  virtual void SendPacket(const Packet& packet,
                          ClientCallback onPacketReceived) = 0;
};
}  // namespace ese