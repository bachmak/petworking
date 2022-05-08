#include "client/tcp/client.h"
#include "client/udp/client.h"
#include "common/logger.h"
#include "common/utils.h"

std::unique_ptr<ese::Client> CreateClient(
    ese::Context& context, const ese::ConnectionSettings& settings,
    ese::Logger& logger) {
  if (settings.protocol == ese::Protocol::TCP) {
    return std::make_unique<ese::tcp::Client>(context, settings.host,
                                              settings.port, logger);
  } else {
    return std::make_unique<ese::udp::Client>(context, settings.host,
                                              settings.port, logger);
  }
}

int main(int argc, char const* argv[]) {
  auto context = ese::Context();
  auto logger = ese::Logger(std::clog, std::cin);

  auto client =
      CreateClient(context, ese::ReadConnectionSettings(argc, argv), logger);

  client->Start();
  context.run();

  return 0;
}
