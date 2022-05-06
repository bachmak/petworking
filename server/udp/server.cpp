#include "server/udp/server.h"

#include "common/logger.h"
#include "common/utils.h"

namespace ese {
namespace udp {

Server::Server(Context& context, const Ip& host, Port port, Logger& logger)
    : socket_(context, Endpoint(host, port)), logger_(logger) {}

void Server::Start() {}

void Server::OnRead(ErrorCode ec, std::size_t bytes_read) {}

void Server::OnWrite(ErrorCode ec, std::size_t bytes_write) {}
}  // namespace udp
}  // namespace ese
