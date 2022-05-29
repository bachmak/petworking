#include "server/tcp/server.h"

#include "common/logger.h"
#include "common/utils.h"
#include "server/tcp/connection.h"

namespace ese::tcp::server {

Server::Server(const Ip& host, Port port, OnAccepted on_connected,
               OnPacketSent on_packet_sent, OnPacketReceived on_packet_received,
               Logger& logger)
    : acceptor_(context_, Endpoint(host, port)),
      on_accepted_(std::move(on_connected)),
      on_packet_sent_(std::move(on_packet_sent)),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger) {}

void Server::Accept() {
  acceptor_.async_accept([this](ErrorCode ec, Socket sock) {
    OnAcceptedImpl(ec, std::move(sock));
  });
}

void Server::OnAcceptedImpl(ErrorCode ec, Socket socket) {
  if (ec) {
    ESE_LOG_EC(logger_, ec);
    return;
  }

  auto connection = std::make_shared<Connection>(
      std::move(socket), on_packet_sent_, on_packet_received_, logger_);

  on_accepted_(*this, *connection);
}
}  // namespace ese::tcp::server
