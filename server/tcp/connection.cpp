#include "server/tcp/connection.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese {
namespace tcp {

Connection::Connection(Socket socket, Logger& logger,
                       ServerCallbackPtr on_packet_received)
    : socket_(std::move(socket)),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger),
      request_packet_body_size_(0u) {
  logger_.LogLine("new connection:", socket_);
}

Connection::~Connection() { /*logger_.LogLine("closed", socket_);*/
}

void Connection::Start() { Read(sizeof(std::size_t)); }

void Connection::OnRead(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (std::exchange(request_packet_body_size_, 0u) == 0u) {
    request_packet_body_size_ = utils::ReadSize(buffer_);
    Read(request_packet_body_size_);
  } else {
    auto request = utils::ReadPacket(buffer_);
    Packet response = on_packet_received_->operator()(request);

    auto response_packet_size = utils::WritePacketWithSize(buffer_, response);
    Write(response_packet_size);
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
}  // namespace tcp
}  // namespace ese