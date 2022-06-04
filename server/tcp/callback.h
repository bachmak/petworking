#pragma once

namespace ese::tcp::server {

class Callback {
 public:
  virtual ~Callback() = default;

 public:
  virtual void OnAccepted(Server& server, Connection& connection) = 0;

  virtual void OnPacketSent(Connection& connection) = 0;

  virtual void OnPacketReceived(Packet packet, Connection& connection) = 0;
};
}  // namespace ese::tcp::server
