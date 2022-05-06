#include "common/logger.h"
#include "common/utils.h"
#include "server/tcp/server.h"
#include "server/udp/server.h"

int main(int argc, char const* argv[]) {
  auto context = ese::Context();
  auto logger = ese::Logger(std::clog, std::cin);

  auto [protocol, host, port] = ese::ReadConnectionSettings(argc, argv);

  auto server = [&]() -> std::unique_ptr<ese::Server> {
    if (protocol == ese::Protocol::TCP) {
      return std::make_unique<ese::tcp::Server>(context, host, port, logger);
    } else {
      return std::make_unique<ese::udp::Server>(context, host, port, logger);
    }
  }();

  server->Start();

  context.run();
  return 0;
}
