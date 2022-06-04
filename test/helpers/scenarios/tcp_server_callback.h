#pragma once

#include "server/tcp/callback.h"
#include "server/tcp/connection.h"
#include "server/tcp/server.h"

namespace ese::test::scenario {

class TcpServerCallback : public tcp::server::Callback {
 public:
  void OnAccepted(tcp::server::Server& server,
                  tcp::server::Connection& connection) override {
    server.Accept();
    connection.Receive();
  }

  void OnPacketSent(tcp::server::Connection& connection) override {
    connection.Receive();
  }

  void OnPacketReceived(Packet packet,
                        tcp::server::Connection& connection) override {
    connection.Send(packet);
  }
};
}  // namespace ese::test::scenario
