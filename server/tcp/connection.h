#pragma once

#include "forwards.h"

namespace ese {
namespace tcp {

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  explicit Connection(Socket socket, Logger& logger,
                      ServerCallbackPtr on_packet_received);
  ~Connection();

  void Start();

 private:
  void OnRead(ErrorCode ec, std::size_t bytes_read, std::size_t bytes_left);

  void OnWrite(ErrorCode ec, std::size_t bytes_written, std::size_t bytes_left);

  void Read(std::size_t bytes);

  void Write(std::size_t bytes);

 private:
  Socket socket_;
  StreamBuf buffer_;
  const ServerCallbackPtr on_packet_received_;
  Logger& logger_;
};
}  // namespace tcp
}  // namespace ese