#pragma once

#include "server/server.h"

namespace ese {
namespace udp {

class Server : public ese::Server {
 public:
  explicit Server(Context& context, const Ip& host, Port port, Logger& logger);

 public:
  void Start() override;

 private:
  void Read();

  void Write(std::size_t bytes_to_write);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void OnWrite(ErrorCode ec, std::size_t bytes_write);

 private:
  Socket socket_;
  Endpoint endpoint_;
  std::array<char, 1024> buffer_;
  Logger& logger_;
};
}  // namespace udp
}  // namespace ese