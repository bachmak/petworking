#pragma once

#include "forwards.h"

namespace ese {

class Server {
 public:
  explicit Server(Context& context, const std::string& host,
                  const std::string& port, Logger& logger);

 private:
  void OnAccepted(ErrorCode ec, Socket socket);

 private:
  Acceptor acceptor_;
  Logger& logger_;
};
}  // namespace ese