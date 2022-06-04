#pragma once

#include "forwards.h"

namespace ese::udp::server {

class Callback {
 public:
  virtual ~Callback() = default;

 public:
  virtual void OnPacketSent(Server& server) = 0;

  virtual void OnPacketReceived(Packet packet, Server& server) = 0;
};
}  // namespace ese::udp::server
