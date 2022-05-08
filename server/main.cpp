#include "common/logger.h"
#include "common/utils.h"
#include "server/tcp/server.h"
#include "server/udp/server.h"

std::unique_ptr<ese::Server> CreateServer(
    ese::Context& context, const ese::ConnectionSettings& settings,
    ese::Logger& logger) {
  if (settings.protocol == ese::Protocol::TCP) {
    return std::make_unique<ese::tcp::Server>(context, settings.host,
                                              settings.port, logger);
  } else {
    return std::make_unique<ese::udp::Server>(context, settings.host,
                                              settings.port, logger);
  }
}

int main(int argc, char const* argv[]) {
  auto context = ese::Context();
  auto logger = ese::Logger(std::clog, std::cin);

  auto server =
      CreateServer(context, ese::ReadConnectionSettings(argc, argv), logger);

  server->Start();

  context.run();
  return 0;
}
