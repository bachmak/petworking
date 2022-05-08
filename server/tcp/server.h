#pragma once

#include "server/server.h"

namespace ese {
namespace tcp {

class Server : public ese::Server {
 public:
  explicit Server(Context& context, const Ip& host, Port port, Logger& logger);

 public:
  void Start() override;

 private:
  void OnAccepted(ErrorCode ec, Socket socket);

 private:
  Acceptor acceptor_;
  Logger& logger_;
};
}  // namespace tcp
}  // namespace ese