#include "server/udp/server.h"

#include "common/logger.h"
#include "common/utils.h"

namespace ese {
namespace udp {

Server::Server(Context& context, const Ip& host, Port port, Logger& logger)
    : socket_(context, Endpoint(host, port)), logger_(logger) {}

void Server::Start() {
  logger_.LogLine("waiting for message...");
  Read();
}

void Server::Read() {
  socket_.async_receive_from(boost::asio::buffer(buffer_), endpoint_,
                             [this](auto&&... args) { OnRead(ESE_FWD(args)); });
}

void Server::Write(std::size_t bytes_to_write) {
  socket_.async_send_to(boost::asio::buffer(buffer_.data(), bytes_to_write),
                        endpoint_,
                        [this](auto&&... args) { OnWrite(ESE_FWD(args)); });
}

void Server::OnRead(ErrorCode ec, std::size_t bytes_read) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (bytes_read > 0) {
    auto message = std::string_view(buffer_.data(), bytes_read);
    logger_.Log("->", message);
  }

  Write(bytes_read);
}

void Server::OnWrite(ErrorCode ec, std::size_t bytes_write) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  Read();
}
}  // namespace udp
}  // namespace ese
