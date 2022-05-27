#pragma once

#include "forwards.h"

namespace ese::tcp::client {

enum class PacketPart { Size, Body };

class Client {
 public:
  explicit Client(Context& context, const Ip& host, Port host_port,
                  OnConnected on_connected, OnPacketReceived on_packet_received,
                  Logger& logger);

 public:
  void Start();

  void SendPacket(const Packet& packet);

 private:
  void OnConnectedImpl(ErrorCode ec, const Endpoint& endpoint);

  void OnWrite(ErrorCode ec);

  void OnRead(ErrorCode ec);

  void Write(std::size_t bytes);

  void Read(std::size_t bytes);

 private:
  Socket socket_;
  Endpoint endpoint_;
  StreamBuf buffer_;
  const OnConnected on_connected_;
  const OnPacketReceived on_packet_received_;
  Logger& logger_;
  std::size_t packet_body_size_;
};
}  // namespace ese::tcp::client