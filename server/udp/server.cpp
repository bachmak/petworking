#include "server/udp/server.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese {
namespace udp {

Server::Server(Context& context, const Ip& host, Port port, Logger& logger)
    : socket_(context, Endpoint(host, port)), logger_(logger) {
  socket_.set_option(Socket::receive_buffer_size(buffer_.size()));
  socket_.set_option(Socket::send_buffer_size(buffer_.size()));
}

void Server::Start(ServerCallback on_packet_received) {
  on_packet_received_ = std::move(on_packet_received);
  logger_.LogLine("waiting for message...");
  Read();
}

void Server::Read() {
  socket_.async_receive_from(boost::asio::buffer(buffer_), remote_endpoint_,
                             [this](auto&&... args) { OnRead(ESE_FWD(args)); });
}

void Server::Write(std::size_t bytes_to_write) {
  socket_.async_send_to(boost::asio::buffer(buffer_.data(), bytes_to_write),
                        remote_endpoint_,
                        [this](auto&&... args) { OnWrite(ESE_FWD(args)); });
}

void Server::OnRead(ErrorCode ec, std::size_t bytes_read) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  auto request = utils::ReadPacket(buffer_, bytes_read);
  Packet response = on_packet_received_(request);
  auto response_packet_size = utils::WritePacket(buffer_, response);

  Write(response_packet_size);
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
