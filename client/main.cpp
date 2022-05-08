#include "client/client_factory.h"
#include "common/logger.h"
#include "common/utils.h"

int main(int argc, char const* argv[]) {
  try {
    auto context = ese::Context();
    auto logger = ese::Logger(std::cerr, std::cin);

    auto settings = ese::ReadConnectionSettings(argc, argv);

    auto client = ese::CreateClient(context, settings, logger);

    client->Start();
    context.run();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }
}
