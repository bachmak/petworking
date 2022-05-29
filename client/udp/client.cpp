#include "client/udp/client.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese::udp::client {

Client::Client(const Ip& host, Port host_port, OnPacketSent on_packet_sent,
               OnPacketReceived on_packet_received, Logger& logger)
    : socket_(context_),
      endpoint_(host, host_port),
      on_packet_sent_(std::move(on_packet_sent)),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger) {
  socket_.open(boost::asio::ip::udp::v4());
  socket_.set_option(Socket::receive_buffer_size(buffer_.size()));
  socket_.set_option(Socket::send_buffer_size(buffer_.size()));
}

void Client::Send(const Packet& packet) {
  auto packet_size = utils::WritePacket(buffer_, packet);
  Write(packet_size);
}

void Client::Receive() { Read(); }

void Client::Write(std::size_t bytes) {
  auto buffer = boost::asio::buffer(buffer_.data(), bytes);
  socket_.async_send_to(buffer, endpoint_,
                        [this](ErrorCode ec, std::size_t) { OnWrite(ec); });
}

void Client::Read() {
  auto buffer = boost::asio::buffer(buffer_.data(), buffer_.size());
  socket_.async_receive_from(buffer, endpoint_,
                             [this](auto... args) { OnRead(ESE_FWD(args)); });
}

void Client::OnWrite(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  on_packet_sent_(*this);
}

void Client::OnRead(ErrorCode ec, std::size_t bytes_read) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  auto packet = utils::ReadPacket(buffer_, bytes_read);
  on_packet_received_(std::move(packet), *this);
}
}  // namespace ese::udp::client
