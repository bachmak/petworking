#include "client/client.h"
#include "common/logger.h"
#include "common/utils.h"

int main(int argc, char const* argv[]) {
  try {
    auto context = ese::Context();
    auto logger = ese::Logger(std::cerr, std::cin);

    auto [host_name, host_port] = ese::ReadNameWithPort(argc, argv);
    auto client = ese::Client(context, host_name, host_port, logger);

    context.run();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }
}
