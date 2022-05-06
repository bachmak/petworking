#include "client/udp/client.h"

#include "common/logger.h"
#include "common/utils.h"

namespace ese {
namespace udp {

Client::Client(Context& context, const Ip& host, Port host_port, Logger& logger)
    : socket_(context, Endpoint(host, host_port)), logger_(logger) {}

void Client::Start() { Write(); }

void Client::OnWrite(ErrorCode ec, std::size_t) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  Read();
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

  socket_.async_send(boost::asio::buffer(message_),
                     [this](auto&&... args) { OnWrite(ESE_FWD(args)); });
}

void Client::Read() {
  socket_.async_receive(boost::asio::buffer(buffer_),
                        [this](auto... args) { OnRead(ESE_FWD(args)); });
}
}  // namespace udp
}  // namespace ese
