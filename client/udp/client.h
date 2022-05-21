#pragma once

#include "client/client.h"

namespace ese {
namespace udp {

class Client : public ese::Client {
 public:
  explicit Client(Context& context, const Ip& host, Port host_port,
                  Logger& logger);

 public:
  void Start(VoidCallback on_started) override;

  void SendPacket(const Packet& packet,
                  ClientCallback on_packet_received) override;

 private:
  void OnWrite(ErrorCode ec, std::size_t bytes_written,
               ClientCallback on_packet_received);

  void OnRead(ErrorCode ec, std::size_t bytes_read,
              const ClientCallback& on_packet_received);

  void Write(std::size_t bytes, ClientCallback on_packet_received);

  void Read(ClientCallback on_packet_received);

 private:
  Socket socket_;
  Endpoint endpoint_;
  StaticBuf buffer_;
  Logger& logger_;
};
}  // namespace udp
}  // namespace ese