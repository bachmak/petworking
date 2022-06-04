#pragma once

#include "common/operation_poller.h"

namespace ese::tcp::client {

class Client : public OperationPoller {
 public:
  explicit Client(const Ip& host, Port host_port,
                  std::unique_ptr<Callback> callback, Logger& logger);

 public:
  void Connect();

  void Send(const Packet& packet);

  void Receive();

 private:
  void Write(std::size_t bytes);

  void Read(std::size_t bytes);

  void OnConnectedImpl(ErrorCode ec);

  void OnWrite(ErrorCode ec);

  void OnRead(ErrorCode ec);

 private:
  Socket socket_;
  Endpoint endpoint_;
  StreamBuf buffer_;
  const std::unique_ptr<Callback> callback_;
  Logger& logger_;
  std::size_t packet_body_size_;
};
}  // namespace ese::tcp::client