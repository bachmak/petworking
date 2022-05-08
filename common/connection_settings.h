#pragma once

#include "forwards.h"

namespace ese {

enum class Protocol { TCP, UDP };

struct ConnectionSettings {
  ConnectionSettings(std::string_view protocol, std::string_view host,
                     std::string_view port) {
    this->protocol = protocol == "udp" ? Protocol::UDP : Protocol::TCP;
    this->host = Ip::from_string(host.data());
    this->port = boost::lexical_cast<Port>(port);
  }
  Protocol protocol;
  Ip host;
  Port port;
};
}  // namespace ese