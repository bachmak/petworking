#include "test/helpers/scenarios/echo.h"

#include "client/tcp/client.h"
#include "client/udp/client.h"
#include "common/connection_settings.h"
#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"
#include "gtest/gtest.h"
#include "server/tcp/connection.h"
#include "server/tcp/server.h"
#include "server/udp/server.h"

namespace ese::test::scenario {

void EchoUdp(const SettingsProvider& settings_provider,
             const std::size_t packet_count) {
  auto server_context = Context();

  auto connection_settings = ConnectionSettings(settings_provider);

  auto packets = std::vector<Packet>();
  packets.reserve(packet_count);

  auto string_generator = utils::StringGenerator("2390 some \t \n message\n");

  for (std::size_t i = 0; i < packet_count; ++i) {
    packets.emplace_back(PacketType::Message, string_generator);
  }

  auto server_logger = Logger(std::clog);
  auto client_logger = Logger(std::clog);

  auto server_on_packet = [](const Packet& packet,
                             udp::server::Server& server) {
    server.SendPacket(packet);
  };

  auto client_on_packet_sent = [](udp::client::Client& client) {
    client.Receive();
  };

  auto client_on_packet_received = [&packets](const Packet& packet,
                                              udp::client::Client& client) {
    EXPECT_EQ(packet.Body(), packets.back().Body());
    packets.pop_back();
    if (!packets.empty()) {
      client.Send(packets.back());
    }
  };

  auto server = udp::server::Server(server_context, connection_settings.host,
                                    connection_settings.port, server_on_packet,
                                    server_logger);

  auto client = udp::client::Client(
      connection_settings.host, connection_settings.port, client_on_packet_sent,
      client_on_packet_received, client_logger);

  server.Start();

  client.Send(packets.back());

  auto server_thread = std::thread([&] { server_context.run(); });

  client.StartPolling();

  server_context.stop();
  server_thread.join();

  EXPECT_TRUE(packets.empty());
}

void EchoTcp(const SettingsProvider& settings_provider,
             const std::size_t packet_count) {
  auto server_context = Context();

  auto connection_settings = ConnectionSettings(settings_provider);

  auto packets = std::vector<Packet>();
  packets.reserve(packet_count);

  auto string_generator = utils::StringGenerator("2390 some \t \n message\n");

  for (std::size_t i = 0; i < packet_count; ++i) {
    packets.emplace_back(PacketType::Message, string_generator);
  }

  auto server_logger = Logger(std::clog);
  auto client_logger = Logger(std::clog);

  auto server_on_connected = [](tcp::server::Connection& connection) {
    connection.Start();
  };

  auto server_on_received = [](const Packet& packet,
                               tcp::server::Connection& connection) {
    connection.SendPacket(packet);
  };

  auto client_on_connected = [&packets](tcp::client::Client& client) {
    client.Send(packets.back());
  };

  auto client_on_sent = [](tcp::client::Client& client) { client.Receive(); };

  auto client_on_received = [&packets](const Packet& packet,
                                       tcp::client::Client& client) {
    EXPECT_EQ(packet.Body(), packets.back().Body());
    packets.pop_back();
    if (!packets.empty()) {
      client.Send(packets.back());
    }
  };

  auto server = tcp::server::Server(
      server_context, connection_settings.host, connection_settings.port,
      server_on_connected, server_on_received, server_logger);
  auto client = tcp::client::Client(
      connection_settings.host, connection_settings.port, client_on_connected,
      client_on_sent, client_on_received, client_logger);

  server.Start();

  client.Connect();

  auto server_thread = std::thread([&] { server_context.run(); });

  client.StartPolling();

  server_context.stop();
  server_thread.join();

  EXPECT_TRUE(packets.empty());
}
}  // namespace ese::test::scenario