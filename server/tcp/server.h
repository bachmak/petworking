#pragma once

#include "common/operation_poller.h"

namespace ese::tcp::server {

class Server : public OperationPoller {
 public:
  explicit Server(const Ip& host, Port port, OnAccepted on_connected,
                  OnPacketSent on_packet_sent,
                  OnPacketReceived on_packet_received, Logger& logger);

 public:
  void Accept();

 private:
  void OnAcceptedImpl(ErrorCode ec, Socket socket);

 private:
  Acceptor acceptor_;
  const OnAccepted on_accepted_;
  const OnPacketSent on_packet_sent_;
  const OnPacketReceived on_packet_received_;
  Logger& logger_;
};
}  // namespace ese::tcp::server