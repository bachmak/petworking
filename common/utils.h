#pragma once

#include "forwards.h"

namespace ese {
inline std::string GetCurrentTime() {
  using namespace boost::posix_time;
  return to_simple_string(second_clock::local_time());
}

inline auto ReadNameWithPort(int argc, char const* argv[]) {
  auto host_name = argc > 1 ? argv[1] : "";
  auto host_port = argc > 2 ? argv[2] : "";
  return std::make_pair(host_name, host_port);
}
}  // namespace ese

namespace std {
inline std::ostream& operator<<(std::ostream& os,
                                const ese::ResolveResults& results) {
  bool first = true;
  os << '[';
  for (const auto& result : results) {
    os << (!std::exchange(first, false) ? ", " : "")
       << result.endpoint().address().to_string();
  }
  return os << ']';
}

inline std::ostream& operator<<(std::ostream& os,
                                const ese::Endpoint& endpoint) {
  return os << endpoint.address().to_string() << ":" << endpoint.port();
}

inline std::ostream& operator<<(std::ostream& os, const ese::Socket& socket) {
  return os << "socket from local endpoint " << socket.local_endpoint()
            << " to remote endpoint " << socket.remote_endpoint();
}
}  // namespace std