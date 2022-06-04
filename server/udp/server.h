#pragma once

#include "common/operation_poller.h"

namespace ese::udp::server {

class Server : public OperationPoller {
 public:
  explicit Server(const Ip& host, Port port, std::unique_ptr<Callback> callback,
                  Logger& logger);

  ~Server();

 public:
  void Send(const Packet& packet);

  void Receive();

 private:
  void Write(std::size_t bytes_to_write);

  void Read();

  void OnWrite(ErrorCode ec, std::size_t bytes_write);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

 private:
  Socket socket_;
  Endpoint remote_endpoint_;
  StaticBuf buffer_;
  const std::unique_ptr<Callback> callback_;
  Logger& logger_;
};
}  // namespace ese::udp::server