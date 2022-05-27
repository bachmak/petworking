#pragma once

#include "forwards.h"

namespace ese::tcp::server {

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  explicit Connection(Socket socket, const OnPacketReceived& on_packet_received,
                      Logger& logger);
  ~Connection();

  void Start();

  void SendPacket(const Packet& packet);

 private:
  void OnRead(ErrorCode ec);

  void OnWrite(ErrorCode ec);

  void Read(std::size_t bytes);

  void Write(std::size_t bytes);

 private:
  Socket socket_;
  StreamBuf buffer_;
  const OnPacketReceived& on_packet_received_;
  Logger& logger_;
  std::size_t packet_body_size_;
};
}  // namespace ese::tcp::server