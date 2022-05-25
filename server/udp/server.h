#pragma once

#include "server/server.h"

namespace ese {
namespace udp {

class Server : public ese::Server {
 public:
  explicit Server(Context& context, const Ip& host, Port port, Logger& logger);

 public:
  void Start(ServerCallback on_packet_received) override;

 private:
  void Read();

  void Write(std::size_t bytes_to_write);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void OnWrite(ErrorCode ec, std::size_t bytes_write);

 private:
  Socket socket_;
  Endpoint remote_endpoint_;
  std::array<char, 8192> buffer_;
  ServerCallback on_packet_received_;
  Logger& logger_;
};
}  // namespace udp
}  // namespace ese