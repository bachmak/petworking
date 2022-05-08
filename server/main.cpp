#include "common/logger.h"
#include "common/utils.h"
#include "server/server_factory.h"

int main(int argc, char const* argv[]) {
  try {
    auto context = ese::Context();
    auto logger = ese::Logger(std::cerr, std::cin);

    auto settings = ese::ReadConnectionSettings(argc, argv);

    auto server = ese::CreateServer(context, settings, logger);

    server->Start();

    context.run();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }
}
