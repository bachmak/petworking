#include "test/helpers/scenarios/echo.h"

#include "client/client_factory.h"
#include "common/connection_settings.h"
#include "common/logger.h"
#include "gtest/gtest.h"
#include "server/server_factory.h"

namespace ese {
namespace test {
namespace scenario {

void Echo(const SettingsProvider& settingsProvider) {
  auto client_context = Context();
  auto server_context = Context();

  auto connectionSettings = ConnectionSettings(settingsProvider);

  auto messages = std::vector<std::string_view>(
      {"test message\n", "another test message\n", "test\n"});

  auto client_os = std::ostringstream();
  auto client_is = std::stringstream();
  std::ranges::for_each(messages, [&](const auto& msg) { client_is << msg; });
  auto client_logger = Logger(client_os, client_is);

  auto server_os = std::ostringstream();
  auto server_is = std::istringstream();
  auto server_logger = Logger(server_os, server_is);

  auto server = CreateServer(server_context, connectionSettings, server_logger);
  auto client = CreateClient(client_context, connectionSettings, client_logger);

  server->Start();
  client->Start();

  auto server_thread = std::thread([&] { server_context.run(); });

  client_context.run();

  server_context.stop();
  server_thread.join();

  for (const auto& message : messages) {
    EXPECT_NE(client_os.str().find(message), std::string::npos);
    EXPECT_NE(server_os.str().find(message), std::string::npos);
  }
}
}  // namespace scenario
}  // namespace test
}  // namespace ese