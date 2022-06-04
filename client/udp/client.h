#pragma once

#include "common/operation_poller.h"

namespace ese::udp::client {

class Client : public OperationPoller {
 public:
  explicit Client(const Ip& host, Port host_port,
                  std::unique_ptr<Callback> callback, Logger& logger);

 public:
  void Send(const Packet& packet);

  void Receive();

 private:
  void Write(std::size_t bytes);

  void Read();

  void OnWrite(ErrorCode ec);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

 private:
  Socket socket_;
  Endpoint endpoint_;
  StaticBuf buffer_;
  const std::unique_ptr<Callback> callback_;
  Logger& logger_;
};
}  // namespace ese::udp::client