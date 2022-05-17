#pragma once

#include "client/client.h"

namespace ese {
namespace udp {

class Client : public ese::Client {
 public:
  explicit Client(Context& context, const Ip& host, Port host_port,
                  Logger& logger);

 public:
  void Start(std::function<void()> callback) override;

  void SendMessage(std::string message,
                   std::function<void(std::string)> callback) override;

 private:
  void OnWrite(ErrorCode ec, std::size_t bytes_written);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void Write();

  void Read();

 private:
  Socket socket_;
  Endpoint endpoint_;
  StaticBuf buffer_;
  Logger& logger_;
};
}  // namespace udp
}  // namespace ese