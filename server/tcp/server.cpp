#include "server/tcp/server.h"

#include "common/logger.h"
#include "common/utils.h"
#include "server/tcp/connection.h"

namespace ese {
namespace tcp {

Server::Server(Context& context, const Ip& host, Port port, Logger& logger)
    : acceptor_(context, Endpoint(host, port)), logger_(logger) {}

void Server::Start() {
  logger_.LogLine("waiting for connections...");
  auto on_accepted = [this](ErrorCode ec, Socket sock) {
    OnAccepted(ec, std::move(sock));
  };
  acceptor_.async_accept(on_accepted);
}

void Server::OnAccepted(ErrorCode ec, Socket socket) {
  if (ec) {
    ESE_LOG_EC(logger_, ec);
    return;
  }

  acceptor_.async_accept([this](auto&&... args) { OnAccepted(ESE_FWD(args)); });

  auto connection = std::make_shared<Connection>(std::move(socket), logger_);
  connection->Start();
}
}  // namespace tcp
}  // namespace ese
