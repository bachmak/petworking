#pragma once

#include "forwards.h"

namespace ese::tcp::client {

class Callback {
 public:
  virtual ~Callback() = default;

 public:
  virtual void OnConnected(Client& client) = 0;

  virtual void OnPacketSent(Client& client) = 0;

  virtual void OnPacketReceived(Packet packet, Client& client) = 0;
};
}  // namespace ese::tcp::client
