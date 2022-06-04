#include "server/tcp/connection.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"
#include "server/tcp/callback.h"

namespace ese::tcp::server {

Connection::Connection(Socket socket, Callback& callback, Logger& logger)
    : socket_(std::move(socket)),
      callback_(callback),
      logger_(logger),
      packet_body_size_(0u) {}

void Connection::Send(const Packet& packet) {
  auto packet_size = utils::WritePacketWithSize(buffer_, packet);
  Write(packet_size);
}

void Connection::Receive() { Read(sizeof(std::size_t)); }

void Connection::Write(std::size_t bytes) {
  boost::asio::async_write(
      socket_, buffer_, boost::asio::transfer_exactly(bytes),
      [connection = shared_from_this()](ErrorCode ec, std::size_t) {
        connection->OnWrite(ec);
      });
}

void Connection::Read(std::size_t bytes) {
  boost::asio::async_read(
      socket_, buffer_, boost::asio::transfer_exactly(bytes),
      [connection = shared_from_this()](ErrorCode ec, std::size_t) {
        connection->OnRead(ec);
      });
}

void Connection::OnWrite(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  callback_.OnPacketSent(*this);
}

void Connection::OnRead(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (std::exchange(packet_body_size_, 0u) == 0u) {
    packet_body_size_ = utils::ReadSize(buffer_);
    Read(packet_body_size_);
  } else {
    auto packet = utils::ReadPacket(buffer_);
    callback_.OnPacketReceived(std::move(packet), *this);
  }
}
}  // namespace ese::tcp::server