#include "client/udp/client.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese::udp::client {

Client::Client(Context& context, const Ip& host, Port host_port,
               OnPacketReceived on_packet_received, Logger& logger)
    : socket_(context),
      endpoint_(host, host_port),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger) {
  socket_.open(boost::asio::ip::udp::v4());
  socket_.set_option(Socket::receive_buffer_size(buffer_.size()));
  socket_.set_option(Socket::send_buffer_size(buffer_.size()));
}

void Client::Start() {}

void Client::SendPacket(const Packet& packet) {
  auto packet_size = utils::WritePacket(buffer_, packet);
  Write(packet_size);
}

void Client::OnWrite(ErrorCode ec) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  Read();
}

void Client::OnRead(ErrorCode ec, std::size_t bytes_read) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  auto packet = utils::ReadPacket(buffer_, bytes_read);
  on_packet_received_(std::move(packet), *this);
}

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
}  // namespace ese::udp::client
