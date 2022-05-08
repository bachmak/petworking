#include "client/client_factory.h"
#include "common/logger.h"
#include "common/utils.h"

int main(int argc, char const* argv[]) {
  auto context = ese::Context();
  auto logger = ese::Logger(std::clog, std::cin);

  auto settings = ese::ReadConnectionSettings(argc, argv);

  auto client = CreateClient(context, settings, logger);

  client->Start();
  context.run();

  return 0;
}
