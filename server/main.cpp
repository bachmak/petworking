#include "common/logger.h"
#include "common/utils.h"
#include "server/server.h"

int main(int argc, char const* argv[]) {
  try {
    auto context = ese::Context();
    auto logger = ese::Logger(std::cerr, std::cin);

    auto [host, port] = ese::ReadNameWithPort(argc, argv);
    auto server = ese::Server(context, host, port, logger);

    context.run();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }
}
