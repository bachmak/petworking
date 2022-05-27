#pragma once

#include "forwards.h"

namespace ese::utils {

inline std::string GetCurrentTime() {
  using namespace boost::posix_time;
  return to_simple_string(second_clock::local_time());
}

std::size_t WritePacketWithSize(StreamBuf& buffer, const Packet& packet);

std::size_t WritePacket(StaticBuf& buffer, const Packet& packet);

std::size_t ReadSize(StreamBuf& buffer);

Packet ReadPacket(StreamBuf& buffer);

Packet ReadPacket(StaticBuf& buffer, std::size_t size);

class StringGenerator {
 public:
  explicit StringGenerator(auto&&... args)
      : gen_(rd_()), string_(ESE_FWD(args)) {}

 public:
  operator std::string() const {
    std::shuffle(string_.begin(), string_.end(), gen_);
    return string_;
  }

 private:
  mutable std::random_device rd_;
  mutable std::mt19937 gen_;
  mutable std::string string_;
};
}  // namespace ese::utils

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