#pragma once

#include "forwards.h"

namespace ese {

inline std::string GetCurrentTime() {
  using namespace boost::posix_time;
  return to_simple_string(second_clock::local_time());
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