#pragma once

#include "forwards.h"

namespace ese::udp::client {

class Client {
 public:
  explicit Client(Context& context, const Ip& host, Port host_port,
                  OnPacketReceived on_packet_received, Logger& logger);

 public:
  void Start();

  void SendPacket(const Packet& packet);

 private:
  void OnWrite(ErrorCode ec);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void Write(std::size_t bytes);

  void Read();

 private:
  Socket socket_;
  Endpoint endpoint_;
  StaticBuf buffer_;
  const OnPacketReceived on_packet_received_;
  Logger& logger_;
};
}  // namespace ese::udp::client