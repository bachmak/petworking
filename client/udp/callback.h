#pragma once

#include "forwards.h"

namespace ese::udp::client {

class Callback {
 public:
  virtual ~Callback() = default;

 public:
  virtual void OnPacketSent(Client& client) = 0;

  virtual void OnPacketReceived(Packet packet, Client& client) = 0;
};
}  // namespace ese::udp::client
