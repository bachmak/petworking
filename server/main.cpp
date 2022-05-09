#include "common/command_line_options.h"
#include "common/connection_settings.h"
#include "common/logger.h"
#include "server/server_factory.h"

int main(int argc, char const* argv[]) {
  try {
    auto cmd_options = ese::CommandLineOptions(argc, argv);

    if (cmd_options.Help()) {
      std::cout << cmd_options << std::endl;
      return 0;
    }

    auto settings = ese::ConnectionSettings(cmd_options);
    auto context = ese::Context();
    auto logger = ese::Logger(std::cerr, std::cin);

    auto server = ese::CreateServer(context, settings, logger);

    server->Start();

    context.run();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }
}
