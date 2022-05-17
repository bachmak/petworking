#pragma once

#include "client/client.h"

namespace ese {
namespace tcp {

enum class PacketPart { Size, Body };

class Client : public ese::Client {
 public:
  explicit Client(Context& context, const Ip& host, Port host_port,
                  Logger& logger);

 public:
  void Start(std::function<void()> callback) override;

  void SendMessage(std::string message,
                   std::function<void(std::string)> callback) override;

 private:
  void OnConnected(ErrorCode ec, const Endpoint& endpoint,
                   std::function<void()> callback);

  void OnWrite(ErrorCode ec, std::size_t bytes_written, std::size_t bytes_left);

  void OnRead(ErrorCode ec, std::size_t bytes_read, std::size_t bytes_left,
              PacketPart packet_part);

  void Write(std::size_t bytes);

  void Read(std::size_t bytes, PacketPart packet_part);

 private:
  Socket socket_;
  Endpoint endpoint_;
  StreamBuf buffer_;
  Logger& logger_;
};
}  // namespace tcp
}  // namespace ese