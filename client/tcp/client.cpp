#include "client/tcp/client.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese::tcp::client {

Client::Client(const Ip& host, Port host_port, OnConnected on_connected,
               OnPacketSent on_packet_sent, OnPacketReceived on_packet_received,
               Logger& logger)
    : socket_(context_),
      endpoint_(host, host_port),
      on_connected_(std::move(on_connected)),
      on_packet_sent_(std::move(on_packet_sent)),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger),
      packet_body_size_(0u) {}

void Client::Connect() {
  auto endpoints = std::vector({endpoint_});
  boost::asio::async_connect(
      socket_, endpoints,
      [this](ErrorCode ec, const Endpoint&) { OnConnectedImpl(ec); });
}

void Client::Send(const Packet& packet) {
  auto packet_size = utils::WritePacketWithSize(buffer_, packet);
  Write(packet_size);
}

void Client::Receive() { Read(sizeof(std::size_t)); }

void Client::Write(std::size_t bytes) {
  boost::asio::async_write(socket_, buffer_,
                           boost::asio::transfer_exactly(bytes),
                           [this](ErrorCode ec, std::size_t) { OnWrite(ec); });
}

void Client::Read(std::size_t bytes) {
  boost::asio::async_read(socket_, buffer_,
                          boost::asio::transfer_exactly(bytes),
                          [this](ErrorCode ec, std::size_t) { OnRead(ec); });
}

void Client::OnConnectedImpl(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  on_connected_(*this);
}

void Client::OnWrite(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  on_packet_sent_(*this);
}

void Client::OnRead(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (std::exchange(packet_body_size_, 0u) == 0u) {
    packet_body_size_ = utils::ReadSize(buffer_);
    Read(packet_body_size_);
  } else {
    auto packet = utils::ReadPacket(buffer_);
    on_packet_received_(std::move(packet), *this);
  }
}
}  // namespace ese::tcp::client
