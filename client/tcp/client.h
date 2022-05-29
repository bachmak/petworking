#pragma once

#include "common/operation_poller.h"

namespace ese::tcp::client {

class Client : public OperationPoller {
 public:
  explicit Client(const Ip& host, Port host_port, OnConnected on_connected,
                  OnPacketSent on_packet_sent,
                  OnPacketReceived on_packet_received, Logger& logger);

 public:
  void Connect();

  void Send(const Packet& packet);

  void Receive();

 private:
  void OnConnectedImpl(ErrorCode ec);

  void OnWrite(ErrorCode ec);

  void OnRead(ErrorCode ec);

  void Write(std::size_t bytes);

  void Read(std::size_t bytes);

 private:
  Socket socket_;
  Endpoint endpoint_;
  StreamBuf buffer_;
  const OnConnected on_connected_;
  const OnPacketSent on_packet_sent_;
  const OnPacketReceived on_packet_received_;
  Logger& logger_;
  std::size_t packet_body_size_;
};
}  // namespace ese::tcp::client