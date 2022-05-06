#pragma once

#include "client/client.h"

namespace ese {
namespace udp {

class Client : public ese::Client {
 public:
  explicit Client(Context& context, const Ip& host, Port host_port,
                  Logger& logger);

 private:
  void Start() override;

  void OnConnected(ErrorCode ec);

  void OnWrite(ErrorCode ec, std::size_t bytes_written);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void Write();

  void Read();

 private:
  Socket socket_;
  std::array<char, 1024> buffer_;
  std::string message_;
  Logger& logger_;
};
}  // namespace udp
}  // namespace ese