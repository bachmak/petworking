#pragma once

#include "common/command_line_options.h"

namespace ese {
namespace detail {
template <typename T>
concept ConnectionSettingsProvider = requires(T t) {
  { t.Protocol() } -> std::convertible_to<std::string>;
  { t.Host() } -> std::convertible_to<std::string>;
  { t.Port() } -> std::convertible_to<std::string>;
};
}  // namespace detail

enum class Protocol { TCP, UDP };

struct ConnectionSettings {
  template <typename T>
  ConnectionSettings(
      const T& settingsProvider) requires detail::ConnectionSettingsProvider<T>
      : protocol(settingsProvider.Protocol() == "udp" ? Protocol::UDP
                                                      : Protocol::TCP),
        host(Ip::from_string(settingsProvider.Host())),
        port(boost::lexical_cast<Port>(settingsProvider.Port())) {}

  Protocol protocol;
  Ip host;
  Port port;
};
}  // namespace ese