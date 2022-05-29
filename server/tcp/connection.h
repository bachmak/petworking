#pragma once

#include "forwards.h"

namespace ese::tcp::server {

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  explicit Connection(Socket socket, const OnPacketSent& on_packet_sent,
                      const OnPacketReceived& on_packet_received,
                      Logger& logger);

 public:
  void Send(const Packet& packet);

  void Receive();

 private:
  void Write(std::size_t bytes);

  void Read(std::size_t bytes);

  void OnWrite(ErrorCode ec);

  void OnRead(ErrorCode ec);

 private:
  Socket socket_;
  StreamBuf buffer_;
  const OnPacketSent& on_packet_sent_;
  const OnPacketReceived& on_packet_received_;
  Logger& logger_;
  std::size_t packet_body_size_;
};
}  // namespace ese::tcp::server