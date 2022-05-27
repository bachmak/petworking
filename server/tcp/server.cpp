#include "server/tcp/server.h"

#include "common/logger.h"
#include "common/utils.h"
#include "server/tcp/connection.h"

namespace ese::tcp::server {

Server::Server(Context& context, const Ip& host, Port port,
               OnConnected on_connected, OnPacketReceived on_packet_received,
               Logger& logger)
    : acceptor_(context, Endpoint(host, port)),
      on_connected_(std::move(on_connected)),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger) {}

void Server::Start() {
  logger_.LogLine("waiting for connections...");

  acceptor_.async_accept(
      [this](ErrorCode ec, Socket sock) { OnAccepted(ec, std::move(sock)); });
}

void Server::OnAccepted(ErrorCode ec, Socket socket) {
  if (ec) {
    ESE_LOG_EC(logger_, ec);
    return;
  }

  acceptor_.async_accept([this](auto&&... args) { OnAccepted(ESE_FWD(args)); });

  auto connection = std::make_shared<Connection>(std::move(socket),
                                                 on_packet_received_, logger_);
  on_connected_(*connection);
}
}  // namespace ese::tcp::server
