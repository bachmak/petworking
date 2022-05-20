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
  void Start(VoidCallback onStarted) override;

  void SendPacket(const Packet& packet,
                  ClientCallback onPacketReceived) override;

 private:
  void OnConnected(ErrorCode ec, const Endpoint& endpoint,
                   VoidCallback onStarted);

  void OnWrite(ErrorCode ec, std::size_t bytes_written, std::size_t bytes_left,
               ClientCallback onPacketReceived);

  void OnRead(ErrorCode ec, std::size_t bytes_read, std::size_t bytes_left,
              ClientCallback onPacketReceived);

  void Write(std::size_t bytes, ClientCallback onPacketReceived);

  void Read(std::size_t bytes, ClientCallback onPacketReceived);

 private:
  Socket socket_;
  Endpoint endpoint_;
  StreamBuf buffer_;
  Logger& logger_;
};
}  // namespace tcp
}  // namespace ese