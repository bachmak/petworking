#include "server/udp/server.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese::udp::server {

Server::Server(Context& context, const Ip& host, Port port,
               OnPacketReceived on_packet_received, Logger& logger)
    : socket_(context, Endpoint(host, port)),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger) {
  socket_.set_option(Socket::receive_buffer_size(buffer_.size()));
  socket_.set_option(Socket::send_buffer_size(buffer_.size()));
}

void Server::Start() {
  logger_.LogLine("waiting for message...");
  Read();
}

void Server::SendPacket(const Packet& packet) {
  auto packet_size = utils::WritePacket(buffer_, packet);
  Write(packet_size);
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

  auto packet = utils::ReadPacket(buffer_, bytes_read);
  on_packet_received_(std::move(packet), *this);
}

void Server::OnWrite(ErrorCode ec, std::size_t bytes_write) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  Read();
}
}  // namespace ese::udp::server
