#include "test/helpers/scenarios/echo.h"

#include "client/client_factory.h"
#include "common/connection_settings.h"
#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"
#include "gtest/gtest.h"
#include "server/server_factory.h"

namespace ese {
namespace test {
namespace scenario {

void Echo(const SettingsProvider& settings_provider,
          const std::size_t packet_count) {
  auto client_context = Context();
  auto server_context = Context();

  auto connection_settings = ConnectionSettings(settings_provider);

  auto packets = std::vector<Packet>();
  packets.reserve(packet_count);

  auto string_generator = utils::StringGenerator("2390 some \t \n message\n");

  for (std::size_t i = 0; i < packet_count; ++i) {
    packets.emplace_back(PacketType::Message, string_generator);
  }

  auto client_logger = Logger(std::clog);
  auto server_logger = Logger(std::clog);

  auto server =
      CreateServer(server_context, connection_settings, server_logger);
  auto client =
      CreateClient(client_context, connection_settings, client_logger);

  auto server_on_packet = [](const Packet& packet) {
    return Packet(PacketType::Message, packet.Body());
  };

  ClientCallback client_on_packet = [&](const Packet& packet) {
    EXPECT_EQ(packet.Body(), packets.back().Body());
    packets.pop_back();
    if (!packets.empty()) {
      client->SendPacket(packets.back(), client_on_packet);
    }
  };

  auto client_on_connected = [&]() {
    client->SendPacket(packets.back(), client_on_packet);
  };

  server->Start(server_on_packet);
  client->Start(client_on_connected);

  auto server_thread = std::thread([&] { server_context.run(); });

  client_context.run();

  server_context.stop();
  server_thread.join();

  EXPECT_TRUE(packets.empty());
}
}  // namespace scenario
}  // namespace test
}  // namespace ese