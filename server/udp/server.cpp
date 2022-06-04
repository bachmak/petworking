#include "server/udp/server.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"
#include "server/udp/callback.h"

namespace ese::udp::server {

Server::Server(const Ip& host, Port port, std::unique_ptr<Callback> callback,
               Logger& logger)
    : socket_(context_, Endpoint(host, port)),
      callback_(std::move(callback)),
      logger_(logger) {
  socket_.set_option(Socket::receive_buffer_size(buffer_.size()));
  socket_.set_option(Socket::send_buffer_size(buffer_.size()));
}

Server::~Server() = default;

void Server::Send(const Packet& packet) {
  auto packet_size = utils::WritePacket(buffer_, packet);
  Write(packet_size);
}

void Server::Receive() { Read(); }

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

  callback_->OnPacketSent(*this);
}

void Server::OnRead(ErrorCode ec, std::size_t bytes_read) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  auto packet = utils::ReadPacket(buffer_, bytes_read);
  callback_->OnPacketReceived(std::move(packet), *this);
}
}  // namespace ese::udp::server
