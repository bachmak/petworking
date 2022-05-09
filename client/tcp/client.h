#pragma once

#include "client/client.h"

namespace ese {
namespace tcp {

class Client : public ese::Client {
 public:
  explicit Client(Context& context, const Ip& host, Port host_port,
                  Logger& logger);

 public:
  void Start() override;

 private:
  void OnConnected(ErrorCode ec, const Endpoint& endpoint);

  void OnWrite(ErrorCode ec, std::size_t bytes_written);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void Write();

  void Read();

 private:
  Socket socket_;
  Endpoint endpoint_;
  StreamBuf buffer_;
  Logger& logger_;
};
}  // namespace tcp
}  // namespace ese