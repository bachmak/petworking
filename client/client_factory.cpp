#include "client_factory.h"

#include "client/tcp/client.h"
#include "client/udp/client.h"
#include "common/connection_settings.h"

namespace ese {

std::unique_ptr<Client> CreateClient(Context& context,
                                     const ConnectionSettings& settings,
                                     Logger& logger) {
  if (settings.protocol == Protocol::TCP) {
    return std::make_unique<tcp::Client>(context, settings.host, settings.port,
                                         logger);
  } else {
    return std::make_unique<udp::Client>(context, settings.host, settings.port,
                                         logger);
  }
}
}  // namespace ese