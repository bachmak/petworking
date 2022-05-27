#pragma once

#include "forwards.h"

namespace ese::tcp::server {

class Server {
 public:
  explicit Server(Context& context, const Ip& host, Port port,
                  OnConnected on_connected, OnPacketReceived on_packet_received,
                  Logger& logger);

 public:
  void Start();

 private:
  void OnAccepted(ErrorCode ec, Socket socket);

 private:
  Acceptor acceptor_;
  const OnConnected on_connected_;
  const OnPacketReceived on_packet_received_;
  Logger& logger_;
};
}  // namespace ese::tcp::server