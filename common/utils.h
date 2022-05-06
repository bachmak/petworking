#pragma once

#include "connection_settings.h"

namespace ese {
inline std::string GetCurrentTime() {
  using namespace boost::posix_time;
  return to_simple_string(second_clock::local_time());
}

inline auto ReadConnectionSettings(int argc, char const* argv[]) {
  auto protocol = argc > 1 ? argv[1] : "";
  auto host_name = argc > 2 ? argv[2] : "";
  auto host_port = argc > 3 ? argv[3] : "";
  return ConnectionSettings(protocol, host_name, host_port);
}
}  // namespace ese

namespace std {

inline std::ostream& operator<<(std::ostream& os,
                                const ese::tcp::Endpoint& endpoint) {
  return os << endpoint.address().to_string() << ":" << endpoint.port();
}

inline std::ostream& operator<<(std::ostream& os,
                                const ese::tcp::Socket& socket) {
  return os << "socket from local endpoint " << socket.local_endpoint()
            << " to remote endpoint " << socket.remote_endpoint();
}
}  // namespace std