#pragma once

#include "server/udp/callback.h"
#include "server/udp/server.h"

namespace ese::test::scenario {

class UdpServerCallback : public udp::server::Callback {
 public:
  void OnPacketSent(udp::server::Server& server) override { server.Receive(); }

  void OnPacketReceived(Packet packet, udp::server::Server& server) override {
    server.Send(packet);
  }
};
}  // namespace ese::test::scenario
