#include "server/udp/server.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese::udp::server {

Server::Server(const Ip& host, Port port, OnPacketSent on_packet_sent,
               OnPacketReceived on_packet_received, Logger& logger)
    : socket_(context_, Endpoint(host, port)),
      on_packet_sent_(on_packet_sent),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger) {
  socket_.set_option(Socket::receive_buffer_size(buffer_.size()));
  socket_.set_option(Socket::send_buffer_size(buffer_.size()));
}

void Server::Send(const Packet& packet) {
  auto packet_size = utils::WritePacket(buffer_, packet);
  Write(packet_size);
}

void Server::Receive() {
  Read();
}

void Server::Write(std::size_t bytes_to_write) {
  socket_.async_send_to(boost::asio::buffer(buffer_.data(), bytes_to_write),
                        remote_endpoint_,
                        [this](auto&&... args) { OnWrite(ESE_FWD(args)); });
}

void Server::Read() {
  socket_.async_receive_from(boost::asio::buffer(buffer_), remote_endpoint_,
                             [this](auto&&... args) { OnRead(ESE_FWD(args)); });
}

void Server::OnWrite(ErrorCode ec, std::size_t bytes_write) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  on_packet_sent_(*this);
}

void Server::OnRead(ErrorCode ec, std::size_t bytes_read) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  auto packet = utils::ReadPacket(buffer_, bytes_read);
  on_packet_received_(std::move(packet), *this);
}
}  // namespace ese::udp::server
