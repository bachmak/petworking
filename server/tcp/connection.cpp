#include "server/tcp/connection.h"

#include "common/logger.h"
#include "common/utils.h"

namespace ese {
namespace tcp {

Connection::Connection(Socket socket, Logger& logger)
    : socket_(std::move(socket)), logger_(logger) {
  logger_.LogLine("new connection:", socket_);

  auto msg_stream = std::ostringstream();
  msg_stream << "you're connected from " << socket_.remote_endpoint()
             << gMsgTerminator;
  message_ = msg_stream.str();
}

Connection::~Connection() { logger_.LogLine("closed", socket_); }

void Connection::Start() { Write(); }

void Connection::OnRead(ErrorCode ec, std::size_t bytes_read) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (bytes_read > 0) {
    message_ = {std::istreambuf_iterator<char>(&buffer_),
                std::istreambuf_iterator<char>()};
    if (message_.size() <= gMsgTerminator.size()) {
      return;
    }

    logger_.Log(socket_.remote_endpoint(), "<-", message_);
  }

  Write();
}

void Connection::OnWrite(ErrorCode ec, std::size_t) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  Read();
}

void Connection::Read() {
  boost::asio::async_read_until(
      socket_, buffer_, gMsgTerminator,
      [connection = shared_from_this()](auto&&... args) {
        connection->OnRead(ESE_FWD(args));
      });
}

void Connection::Write() {
  boost::asio::async_write(socket_, boost::asio::buffer(message_),
                           [connection = shared_from_this()](auto&&... args) {
                             connection->OnWrite(ESE_FWD(args));
                           });
}
}  // namespace tcp
}  // namespace ese