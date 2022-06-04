#include "server/tcp/server.h"

#include "common/logger.h"
#include "common/utils.h"
#include "server/tcp/callback.h"
#include "server/tcp/connection.h"

namespace ese::tcp::server {

Server::Server(const Ip& host, Port port, std::unique_ptr<Callback> callback,
               Logger& logger)
    : acceptor_(context_, Endpoint(host, port)),
      callback_(std::move(callback)),
      logger_(logger) {}

Server::~Server() = default;

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

  auto connection =
      std::make_shared<Connection>(std::move(socket), *callback_, logger_);

  callback_->OnAccepted(*this, *connection);
}
}  // namespace ese::tcp::server
