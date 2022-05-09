#pragma once

#include "common/command_line_options.h"

namespace ese {

enum class Protocol { TCP, UDP };

struct ConnectionSettings {
  ConnectionSettings(const CommandLineOptions& cmd_options)
      : protocol(cmd_options.Protocol() == "udp" ? Protocol::UDP
                                                 : Protocol::TCP),
        host(Ip::from_string(cmd_options.Host())),
        port(boost::lexical_cast<Port>(cmd_options.Port())) {}

  Protocol protocol;
  Ip host;
  Port port;
};
}  // namespace ese