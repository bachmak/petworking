#include "common/logger.h"
#include "common/utils.h"
#include "server/server.h"

int main(int argc, char const* argv[]) {
  auto context = ese::Context();
  auto logger = ese::Logger(std::cerr, std::cin);

  auto [host, port] = ese::ReadNameWithPort(argc, argv);
  auto server = ese::Server(context, host, port, logger);

  context.run();

  return 0;
}
