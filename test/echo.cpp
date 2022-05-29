#include "test/helpers/scenarios/echo.h"

#include "gtest/gtest.h"

inline constexpr std::size_t gPacketCount = 10000;

TEST(EchoTest, EchoTcp) {
  using namespace ese;

  auto settings_provider = test::SettingsProvider("tcp", "127.0.0.1", "9011");
  test::scenario::EchoTcp(settings_provider, gPacketCount);
}

TEST(EchoTest, EchoUdp) {
  using namespace ese;

  auto settings_provider = test::SettingsProvider("udp", "127.0.0.1", "32743");
  test::scenario::EchoUdp(settings_provider, gPacketCount);
}