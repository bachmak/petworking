#pragma once

#include "client/udp/callback.h"
#include "client/udp/client.h"
#include "gtest/gtest.h"

namespace ese::test::scenario {

class UdpClientCallback : public udp::client::Callback {
 public:
  explicit UdpClientCallback(std::vector<Packet>& packets)
      : packets_(packets) {}

 public:
  void OnPacketSent(udp::client::Client& client) override { client.Receive(); }

  void OnPacketReceived(Packet packet, udp::client::Client& client) override {
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
