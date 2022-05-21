#include "test/helpers/scenarios/echo.h"

#include "gtest/gtest.h"

TEST(EchoTest, EchoTcp) {
  using namespace ese;

  auto settingsProvider = test::SettingsProvider("tcp", "127.0.0.1", "9011");
  test::scenario::Echo(settingsProvider);
}

TEST(EchoTest, EchoUdp) {
  using namespace ese;

  auto settingsProvider = test::SettingsProvider("udp", "127.0.0.1", "32743");
  test::scenario::Echo(settingsProvider);
}