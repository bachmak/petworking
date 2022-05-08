#include "client/tcp/client.h"

#include "common/logger.h"
#include "common/utils.h"

namespace ese {
namespace tcp {

Client::Client(Context& context, const Ip& host, Port host_port, Logger& logger)
    : socket_(context), endpoint_(host, host_port), logger_(logger) {}

void Client::Start() {
  auto endpoints = std::vector({endpoint_});
  boost::asio::async_connect(
      socket_, endpoints,
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
}  // namespace tcp
}  // namespace ese
