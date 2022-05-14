#include "common/connection_settings.h"

namespace ese {
namespace test {

struct SettingsProvider {
  explicit SettingsProvider(std::string protocol, std::string host,
                            std::string port)
      : protocol_(std::move(protocol)),
        host_(std::move(host)),
        port_(std::move(port)) {}

  const std::string& Protocol() const { return protocol_; }
  const std::string& Host() const { return host_; }
  const std::string& Port() const { return port_; }

  const std::string protocol_;
  const std::string host_;
  const std::string port_;
};

namespace scenario {

void Echo(const SettingsProvider& settingsProvider);
}
}  // namespace test
}  // namespace ese