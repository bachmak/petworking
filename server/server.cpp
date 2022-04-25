#include "server/server.h"

#include "common/logger.h"
#include "common/utils.h"
#include "server/connection.h"

namespace ese {

Server::Server(Context& context, const std::string& host,
               const std::string& port, Logger& logger)
    : acceptor_(context, Endpoint(Ip::from_string(host),
                                  boost::lexical_cast<Port>(port))),
      logger_(logger) {
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

  auto connection = std::make_shared<Connection>(std::move(socket), logger_);
  connection->Start();
}
}  // namespace ese
