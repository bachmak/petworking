#include "client/tcp/client.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese {
namespace tcp {

Client::Client(Context& context, const Ip& host, Port host_port, Logger& logger)
    : socket_(context),
      endpoint_(host, host_port),
      logger_(logger),
      response_packet_body_size_(0u) {}

void Client::Start(VoidCallback onStarted) {
  auto endpoints = std::vector({endpoint_});
  boost::asio::async_connect(socket_, endpoints,
                             [this, cb = std::move(onStarted)](
                                 ErrorCode ec, const Endpoint& ep) mutable {
                               OnConnected(ec, ep, std::move(cb));
                             });
}

void Client::SendPacket(const Packet& packet, ClientCallback onPacketReceived) {
  auto request_packet_size = utils::WritePacketWithSize(buffer_, packet);
  Write(request_packet_size, std::move(onPacketReceived));
}

void Client::OnConnected(ErrorCode ec, const Endpoint& endpoint,
                         VoidCallback onStarted) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  onStarted();
}

void Client::OnWrite(ErrorCode ec, ClientCallback onPacketReceived) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  Read(sizeof(std::size_t), std::move(onPacketReceived));
}

void Client::OnRead(ErrorCode ec, ClientCallback onPacketReceived) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (std::exchange(response_packet_body_size_, 0u) == 0u) {
    response_packet_body_size_ = utils::ReadSize(buffer_);
    Read(response_packet_body_size_, std::move(onPacketReceived));
  } else {
    auto packet = utils::ReadPacket(buffer_);
    onPacketReceived(packet);
  }
}

void Client::Write(std::size_t bytes, ClientCallback onPacketReceived) {
  boost::asio::async_write(
      socket_, buffer_, boost::asio::transfer_exactly(bytes),
      [this, cb = std::move(onPacketReceived)](
          ErrorCode ec, std::size_t) mutable { OnWrite(ec, std::move(cb)); });
}

void Client::Read(std::size_t bytes, ClientCallback onPacketReceived) {
  boost::asio::async_read(
      socket_, buffer_, boost::asio::transfer_exactly(bytes),
      [this, cb = std::move(onPacketReceived)](
          ErrorCode ec, std::size_t) mutable { OnRead(ec, std::move(cb)); });
}
}  // namespace tcp
}  // namespace ese
