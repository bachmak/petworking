#pragma once

#include "forwards.h"

namespace ese {
class Client {
 public:
  explicit Client(Context& context, const std::string& host_name,
                  const std::string& host_port, Logger& logger);

 private:
  void OnResolved(ErrorCode ec, ResolveResults resolve_results);

  void OnConnected(ErrorCode ec, const Endpoint& endpoint);

  void OnWrite(ErrorCode ec, std::size_t bytes_written);

  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void Write();

  void Read();

 private:
  Socket socket_;
  Resolver resolver_;
  Buffer buffer_;
  std::string message_;
  Logger& logger_;
};
}  // namespace ese