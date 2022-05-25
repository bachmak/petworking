#include "server/tcp/server.h"

#include "common/logger.h"
#include "common/utils.h"
#include "server/tcp/connection.h"

namespace ese {
namespace tcp {

Server::Server(Context& context, const Ip& host, Port port, Logger& logger)
    : acceptor_(context, Endpoint(host, port)), logger_(logger) {}

void Server::Start(ServerCallback onPacketReceived) {
  logger_.LogLine("waiting for connections...");

  auto cb = std::make_shared<ServerCallback>(std::move(onPacketReceived));
  acceptor_.async_accept([this, cb = std::move(cb)](ErrorCode ec, Socket sock) {
    OnAccepted(ec, std::move(sock), std::move(cb));
  });
}

void Server::OnAccepted(ErrorCode ec, Socket socket,
                        ServerCallbackPtr onPacketReceived) {
  if (ec) {
    ESE_LOG_EC(logger_, ec);
    return;
  }

  acceptor_.async_accept([this, cb = onPacketReceived](auto&&... args) {
    OnAccepted(ESE_FWD(args), std::move(cb));
  });

  auto connection = std::make_shared<Connection>(std::move(socket), logger_,
                                                 std::move(onPacketReceived));
  connection->Start();
}
}  // namespace tcp
}  // namespace ese
