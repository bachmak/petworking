#include "test/helpers/scenarios/echo.h"

#include "client/client_factory.h"
#include "common/connection_settings.h"
#include "common/logger.h"
#include "common/packet.h"
#include "gtest/gtest.h"
#include "server/server_factory.h"

namespace ese {
namespace test {
namespace scenario {

auto ServerOnPacketReceived(Packet packet) {}

void Echo(const SettingsProvider& settingsProvider) {
  testing::internal::CaptureStderr();
  std::cerr << __FUNCTION__ << std::endl;
  std::string cerr = testing::internal::GetCapturedStderr();
  auto client_context = Context();
  auto server_context = Context();

  auto connectionSettings = ConnectionSettings(settingsProvider);

  auto packets =
      std::vector<Packet>({Packet(PacketType::Message, "test message"),
                           Packet(PacketType::Message, "another test message"),
                           Packet(PacketType::Message, "test")});

  auto client_logger = Logger(std::cerr, std::cin);

  auto server_logger = Logger(std::cerr, std::cin);

  auto server = CreateServer(server_context, connectionSettings, server_logger);
  auto client = CreateClient(client_context, connectionSettings, client_logger);

  auto serverOnPacket = [](Packet packet) {
    return Packet(PacketType::Message, packet.Body());
  };

  ClientCallback clientOnPacket = [&](Packet packet) {
    EXPECT_NE(packet.Body(), packets.back().Body());
    packets.pop_back();
    if (!packets.empty()) {
      client->SendPacket(packets.back(), clientOnPacket);
    }
  };

  auto clientOnConnected = [&]() {
    client->SendPacket(packets.back(), clientOnPacket);
    std::cerr << "Packet sent\n";
  };

  server->Start(serverOnPacket);
  client->Start(clientOnConnected);

  auto server_thread = std::thread([&] { server_context.run(); });

  client_context.run();

  server_context.stop();
  server_thread.join();

  EXPECT_TRUE(packets.empty());
}
}  // namespace scenario
}  // namespace test
}  // namespace ese