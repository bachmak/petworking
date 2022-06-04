#pragma once

#include "common/operation_poller.h"

namespace ese::tcp::server {

class Server : public OperationPoller {
 public:
  explicit Server(const Ip& host, Port port, std::unique_ptr<Callback> callback,
                  Logger& logger);

  ~Server();

 public:
  void Accept();

 private:
  void OnAcceptedImpl(ErrorCode ec, Socket socket);

 private:
  Acceptor acceptor_;
  const std::unique_ptr<Callback> callback_;
  Logger& logger_;
};
}  // namespace ese::tcp::server