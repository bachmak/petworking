#include "client/client_factory.h"
#include "common/command_line_options.h"
#include "common/connection_settings.h"
#include "common/logger.h"
#include "gtest/gtest.h"
#include "server/server_factory.h"

TEST(EchoTest, TcpEcho) {
  auto client_context = ese::Context();
  auto server_context = ese::Context();

  struct SettingsProvider {
    std::string Protocol() const { return "tcp"; }
    std::string Host() const { return "127.0.0.1"; }
    std::string Port() const { return "9000"; }
  };

  auto connectionSettings = ese::ConnectionSettings(SettingsProvider());

  auto messages = std::vector<std::string_view>(
      {"test message\n", "another test message\n", "test\n"});

  std::stringstream is;
  for (const auto& message : messages) {
    is << message;
  }

  std::ostringstream os;
  auto logger = ese::Logger(os, is);

  auto server = ese::CreateServer(server_context, connectionSettings, logger);
  auto client = ese::CreateClient(client_context, connectionSettings, logger);

  server->Start();
  client->Start();

  auto server_thread = std::thread([&] { server_context.run(); });
  auto client_thread = std::thread([&] { client_context.run(); });

  client_thread.join();

  server.reset();
  server_thread.join();

  for (const auto& message : messages) {
    EXPECT_NE(os.str().find(message), std::string::npos);
  }
}