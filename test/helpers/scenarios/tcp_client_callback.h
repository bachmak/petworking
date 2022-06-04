#pragma once

#include "client/tcp/callback.h"
#include "client/tcp/client.h"
#include "gtest/gtest.h"

namespace ese::test::scenario {

class TcpClientCallback : public tcp::client::Callback {
 public:
  explicit TcpClientCallback(std::vector<Packet>& packets)
      : packets_(packets) {}

 public:
  void OnConnected(tcp::client::Client& client) override {
    client.Send(packets_.back());
  }

  void OnPacketSent(tcp::client::Client& client) override { client.Receive(); }

  void OnPacketReceived(Packet packet, tcp::client::Client& client) override {
    EXPECT_EQ(packet.Body(), packets_.back().Body());
    packets_.pop_back();
    if (!packets_.empty()) {
      client.Send(packets_.back());
    }
  }

 private:
  std::vector<Packet>& packets_;
};
}  // namespace ese::test::scenario