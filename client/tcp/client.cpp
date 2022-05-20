#include "client/tcp/client.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese {
namespace tcp {

Client::Client(Context& context, const Ip& host, Port host_port, Logger& logger)
    : socket_(context), endpoint_(host, host_port), logger_(logger) {}

void Client::Start(VoidCallback onStarted) {
  std::cerr << __FUNCTION__ << std::endl;
  auto endpoints = std::vector({endpoint_});
  boost::asio::async_connect(
      socket_, endpoints,
      [this, cb = std::move(onStarted)](ErrorCode ec, const Endpoint& ep) {
        OnConnected(ec, ep, std::move(cb));
      });
}

void Client::SendPacket(const Packet& packet, ClientCallback onPacketReceived) {
  std::cerr << __FUNCTION__ << std::endl;
  auto json = nlohmann::json(packet);
  std::cerr << json << std::endl;

  auto os = std::ostream(&buffer_);
  auto body_size = json.size();

  os.write(reinterpret_cast<char*>(&body_size), sizeof(body_size)) << json;

  Write(sizeof(body_size) + json.size(), std::move(onPacketReceived));
}

void Client::OnConnected(ErrorCode ec, const Endpoint& endpoint,
                         VoidCallback onStarted) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  onStarted();
}

void Client::OnWrite(ErrorCode ec, std::size_t bytes_written,
                     std::size_t bytes_left, ClientCallback onPacketReceived) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (bytes_left -= bytes_written; bytes_left == 0) {
    Read(sizeof(nlohmann::json::size_type), std::move(onPacketReceived));
  } else {
    Write(bytes_left, std::move(onPacketReceived));
  }
}

void Client::OnRead(ErrorCode ec, std::size_t bytes_read,
                    std::size_t bytes_left, ClientCallback onPacketReceived) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  static auto packetPart = PacketPart::Size;

  if (std::exchange(packetPart, PacketPart::Body) == PacketPart::Size) {
    auto body_size = nlohmann::json::size_type();
    if (bytes_read >= sizeof(body_size)) {
      auto is = std::istream(&buffer_);
      is.read(reinterpret_cast<char*>(&body_size), sizeof(body_size));

      if (bytes_read -= sizeof(body_size); bytes_read == body_size) {
        auto json = nlohmann::json();
        is >> json;
        std::cout << "Response: " << json << std::endl;
        auto packet = json.get<Packet>();
        return onPacketReceived(std::move(packet));
      }
    } else {
      bytes_left = sizeof(body_size) - bytes_read;
      packetPart = PacketPart::Size;
      return Read(bytes_left, std::move(onPacketReceived));
    }
  }
  return Read(bytes_left - bytes_read, std::move(onPacketReceived));
}

void Client::Write(std::size_t bytes, ClientCallback onPacketReceived) {
  boost::asio::async_write(
      socket_, buffer_,
      [this, bytes, cb = std::move(onPacketReceived)](auto&&... args) {
        OnWrite(ESE_FWD(args), bytes, std::move(cb));
      });
}

void Client::Read(std::size_t bytes, ClientCallback onPacketReceived) {
  boost::asio::async_read(
      socket_, buffer_,
      [this, bytes, cb = std::move(onPacketReceived)](auto&&... args) {
        OnRead(ESE_FWD(args), bytes, std::move(cb));
      });
}
}  // namespace tcp
}  // namespace ese
