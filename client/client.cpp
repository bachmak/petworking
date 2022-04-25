#include "client/client.h"

#include "common/logger.h"
#include "common/utils.h"

namespace ese {
Client::Client(Context& context, const std::string& host_name,
               const std::string& host_port, Logger& logger)
    : socket_(context), resolver_(context), logger_(logger) {
  resolver_.async_resolve(host_name, host_port,
                          [this](auto... args) { OnResolved(ESE_FWD(args)); });
}

void Client::OnResolved(ErrorCode ec, ResolveResults resolve_results) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  logger_.LogLine("resolved", resolve_results);

  boost::asio::async_connect(
      socket_, resolve_results,
      [this](ErrorCode ec, const Endpoint& ep) { OnConnected(ec, ep); });
}

void Client::OnConnected(ErrorCode ec, const Endpoint& endpoint) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  logger_.LogLine("connected to", endpoint);

  Read();
}

void Client::OnWrite(ErrorCode ec, std::size_t) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  boost::asio::async_read_until(
      socket_, buffer_, gMsgTerminator,
      [this](auto... args) { OnRead(ESE_FWD(args)); });
}

void Client::OnRead(ErrorCode ec, std::size_t) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  logger_.Log("->", &buffer_);

  Write();
}

void Client::Write() {
  logger_.Log("<- ");
  logger_.ReadLine(message_);
  message_ += gMsgTerminator;

  boost::asio::async_write(socket_, boost::asio::buffer(message_),
                           [this](auto&&... args) { OnWrite(ESE_FWD(args)); });
}

void Client::Read() {
  boost::asio::async_read_until(
      socket_, buffer_, gMsgTerminator,
      [this](auto&&... args) { OnRead(ESE_FWD(args)); });
}
}  // namespace ese
