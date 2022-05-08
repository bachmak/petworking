#include "client/tcp/client.h"
#include "client/udp/client.h"
#include "common/logger.h"
#include "common/utils.h"

int main(int argc, char const* argv[]) {
  auto context = ese::Context();
  auto logger = ese::Logger(std::clog, std::cin);

  auto [protocol, host_name, host_port] =
      ese::ReadConnectionSettings(argc, argv);

  auto client = [&]() -> std::unique_ptr<ese::Client> {
    if (protocol == ese::Protocol::TCP) {
      return std::make_unique<ese::tcp::Client>(context, host_name, host_port,
                                                logger);
    } else {
      return std::make_unique<ese::udp::Client>(context, host_name, host_port,
                                                logger);
    }
  }();

  client->Start();
  context.run();

  return 0;
}
