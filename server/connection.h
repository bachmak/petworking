#pragma once

#include "forwards.h"

namespace ese {
class Connection : public std::enable_shared_from_this<Connection> {
 public:
  explicit Connection(Socket socket, Logger& logger);
  ~Connection();

  void Start();

 private:
  void OnRead(ErrorCode ec, std::size_t bytes_read);

  void OnWrite(ErrorCode ec, std::size_t bytes_write);

  void Read();

  void Write();

 private:
  Socket socket_;
  Buffer buffer_;
  std::string message_;
  Logger& logger_;
};
}  // namespace ese