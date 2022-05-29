#pragma once

#include "common/operation_poller.h"

namespace ese::udp::client {

class Client : public OperationPoller {
 public:
  explicit Client(const Ip& host, Port host_port, OnPacketSent on_packet_sent,
                  OnPacketReceived on_packet_received, Logger& logger);

 public:
  void Send(const Packet& packet);

  void Receive();

 private:
  void OnWrite(ErrorCode ec);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void Write(std::size_t bytes);

  void Read();

 private:
  Socket socket_;
  Endpoint endpoint_;
  StaticBuf buffer_;
  const OnPacketSent on_packet_sent_;
  const OnPacketReceived on_packet_received_;
  Logger& logger_;
};
}  // namespace ese::udp::client