#pragma once

#include "forwards.h"

namespace ese::udp::server {

class Server {
 public:
  explicit Server(Context& context, const Ip& host, Port port,
                  OnPacketReceived on_packet_received, Logger& logger);

 public:
  void Start();

  void SendPacket(const Packet& packet);

 private:
  void Read();

  void Write(std::size_t bytes_to_write);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void OnWrite(ErrorCode ec, std::size_t bytes_write);

 private:
  Socket socket_;
  Endpoint remote_endpoint_;
  StaticBuf buffer_;
  const OnPacketReceived on_packet_received_;
  Logger& logger_;
};
}  // namespace ese::udp::server