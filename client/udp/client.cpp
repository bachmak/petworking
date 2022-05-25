#include "client/udp/client.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese {
namespace udp {

Client::Client(Context& context, const Ip& host, Port host_port, Logger& logger)
    : socket_(context), endpoint_(host, host_port), logger_(logger) {
  socket_.open(boost::asio::ip::udp::v4());
  socket_.set_option(Socket::receive_buffer_size(buffer_.size()));
  socket_.set_option(Socket::send_buffer_size(buffer_.size()));
}

void Client::Start(VoidCallback on_started) { on_started(); }

void Client::SendPacket(const Packet& packet,
                        ClientCallback on_packet_received) {
  auto request_packet_size = utils::WritePacket(buffer_, packet);
  Write(request_packet_size, on_packet_received);
}

void Client::OnWrite(ErrorCode ec, std::size_t bytes_written,
                     ClientCallback on_packet_received) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  Read(std::move(on_packet_received));
}

void Client::OnRead(ErrorCode ec, std::size_t bytes_read,
                    const ClientCallback& on_packet_received) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  auto response = utils::ReadPacket(buffer_, bytes_read);
  on_packet_received(response);
}

void Client::Write(std::size_t bytes, ClientCallback on_packet_received) {
  auto buffer = boost::asio::buffer(buffer_.data(), bytes);
  socket_.async_send_to(
      buffer, endpoint_,
      [this, cb = std::move(on_packet_received)](auto&&... args) mutable {
        OnWrite(ESE_FWD(args), std::move(cb));
      });
}

void Client::Read(ClientCallback on_packet_received) {
  auto buffer = boost::asio::buffer(buffer_.data(), buffer_.size());
  socket_.async_receive_from(buffer, endpoint_,
                             [this, cb = std::move(on_packet_received)](
                                 auto... args) { OnRead(ESE_FWD(args), cb); });
}
}  // namespace udp
}  // namespace ese
