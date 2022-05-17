#include "client/tcp/client.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese {
namespace tcp {

Client::Client(Context& context, const Ip& host, Port host_port, Logger& logger)
    : socket_(context), endpoint_(host, host_port), logger_(logger) {}

void Client::Start(std::function<void()> callback) {
  auto endpoints = std::vector({endpoint_});
  boost::asio::async_connect(
      socket_, endpoints,
      [this, callback = std::move(callback)](ErrorCode ec, const Endpoint& ep) {
        OnConnected(ec, ep, std::move(callback));
      });
}

void Client::SendMessage(std::string message,
                         std::function<void(std::string)> callback) {
  auto packet = Packet(PacketType::Message, std::move(message));
  auto json = nlohmann::json(packet);

  auto os = std::ostream(&buffer_);
  auto body_size = json.size();

  os.write(reinterpret_cast<char*>(&body_size), sizeof(body_size)) << json;

  Write(sizeof(body_size) + json.size());
}

void Client::OnConnected(ErrorCode ec, const Endpoint& endpoint,
                         std::function<void()> callback) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  callback();
}

void Client::OnWrite(ErrorCode ec, std::size_t bytes_written,
                     std::size_t bytes_left) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (bytes_left -= bytes_written; bytes_left == 0) {
    Read(sizeof(nlohmann::json::size_type), PacketPart::Size);
  } else {
    Write(bytes_left);
  }
}

void Client::OnRead(ErrorCode ec, std::size_t bytes_read,
                    std::size_t bytes_left, PacketPart packet_part) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (packet_part == PacketPart::Size) {
    if (bytes_read >= bytes_left) {
      auto body_size = nlohmann::json::size_type();
      auto is = std::istream(&buffer_);
      is.read(reinterpret_cast<char*>(&body_size), sizeof(body_size));
      if (bytes_read -= sizeof(body_size); bytes_read < body_size) {
        Read(body_size - bytes_read, PacketPart::Body);
      } else {
        auto packet = Packet();
        auto json = nlohmann::json(packet);
        is >> json;
        if (packet.Type() == PacketType::Message) {
        }
      }
    } else {
      bytes_left -= bytes_read;
      Read(bytes_left, PacketPart::Size);
    }
  } else {
    // Read(bytes_left, );
  }
}

void Client::Write(std::size_t bytes) {
  boost::asio::async_write(socket_, buffer_, [this, bytes](auto&&... args) {
    OnWrite(ESE_FWD(args), bytes);
  });
}

void Client::Read(std::size_t bytes, PacketPart packet_part) {
  boost::asio::async_read(socket_, buffer_,
                          [this, bytes, packet_part](auto&&... args) {
                            OnRead(ESE_FWD(args), bytes, packet_part);
                          });
}
}  // namespace tcp
}  // namespace ese
