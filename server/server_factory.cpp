#include "server/server_factory.h"

#include "common/connection_settings.h"
#include "server/tcp/server.h"
#include "server/udp/server.h"

namespace ese {

std::unique_ptr<Server> CreateServer(Context& context,
                                     const ConnectionSettings& settings,
                                     Logger& logger) {
  if (settings.protocol == Protocol::TCP) {
    return std::make_unique<tcp::Server>(context, settings.host, settings.port,
                                         logger);
  } else {
    return std::make_unique<udp::Server>(context, settings.host, settings.port,
                                         logger);
  }
}
}  // namespace ese