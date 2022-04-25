#include "client/client.h"
#include "common/logger.h"
#include "common/utils.h"

int main(int argc, char const* argv[]) {
  auto context = ese::Context();
  auto logger = ese::Logger(std::clog, std::cin);

  auto [host_name, host_port] = ese::ReadNameWithPort(argc, argv);
  auto client = ese::Client(context, host_name, host_port, logger);

  context.run();

  return 0;
}
