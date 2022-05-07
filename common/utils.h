#pragma once

#include "forwards.h"

namespace ese {
inline std::string GetCurrentTime() {
  using namespace boost::posix_time;
  return to_simple_string(second_clock::local_time());
}

inline auto ReadNameWithPort(int argc, char const* argv[]) {
  namespace po = boost::program_options;

  auto description = po::options_description("All options");

  auto host_opt = po::value<std::string>()->default_value("127.0.0.1");
  auto port_opt = po::value<std::string>()->default_value("8001");

  description.add_options()("help", "produce help message");
  description.add_options()("host,h", host_opt, "host to connect/accept");
  description.add_options()("port,p", port_opt, "port to connect/accept");

  auto vm = po::variables_map();
  po::store(po::parse_command_line(argc, argv, description), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << description << "\n";
    std::exit(1);
  }

  auto host_name = vm["host"].as<std::string>();
  auto host_port = vm["port"].as<std::string>();
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