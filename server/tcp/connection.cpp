#include "server/tcp/connection.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese::tcp::server {

Connection::Connection(Socket socket,
                       const OnPacketReceived& on_packet_received,
                       Logger& logger)
    : socket_(std::move(socket)),
      on_packet_received_(on_packet_received),
      logger_(logger),
      packet_body_size_(0u) {
  logger_.LogLine("new connection:", socket_);
}

Connection::~Connection() { /*logger_.LogLine("closed", socket_);*/
}

void Connection::Start() { Read(sizeof(std::size_t)); }

void Connection::SendPacket(const Packet& packet) {
  auto packet_size = utils::WritePacketWithSize(buffer_, packet);
  Write(packet_size);
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
    on_packet_received_(std::move(packet), *this);
  }
}

void Connection::OnWrite(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  Read(sizeof(std::size_t));
}

void Connection::Read(std::size_t bytes) {
  boost::asio::async_read(
      socket_, buffer_, boost::asio::transfer_exactly(bytes),
      [connection = shared_from_this()](ErrorCode ec, std::size_t) {
        connection->OnRead(ec);
      });
}

void Connection::Write(std::size_t bytes) {
  boost::asio::async_write(
      socket_, buffer_, boost::asio::transfer_exactly(bytes),
      [connection = shared_from_this()](ErrorCode ec, std::size_t) {
        connection->OnWrite(ec);
      });
}
}  // namespace ese::tcp::server