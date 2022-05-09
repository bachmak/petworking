#include "common/command_line_options.h"

namespace ese {

CommandLineOptions::CommandLineOptions(int argc, char const* argv[])
    : description_(CreateDescription()),
      variables_map_(ParseVariablesMap(argc, argv)) {}

std::ostream& operator<<(std::ostream& os, const CommandLineOptions& options) {
  options.description_.print(os);
  return os;
}

bool CommandLineOptions::Help() const {
  return variables_map_.count("help") > 0u;
}

const std::string& CommandLineOptions::Protocol() const {
  return variables_map_["protocol"].as<std::string>();
}

const std::string& CommandLineOptions::Port() const {
  return variables_map_["port"].as<std::string>();
}

const std::string& CommandLineOptions::Host() const {
  return variables_map_["host"].as<std::string>();
}

CommandLineOptions::DescriptionType CommandLineOptions::CreateDescription() {
  namespace po = boost::program_options;

  auto description = DescriptionType("All options");

  auto protocol_opt = po::value<std::string>()->default_value("tcp");
  auto host_opt = po::value<std::string>()->default_value("127.0.0.1");
  auto port_opt = po::value<std::string>()->default_value("8001");

  description.add_options()("help", "produce help message");
  description.add_options()("protocol,t", protocol_opt, "transport protocol");
  description.add_options()("host,h", host_opt, "host to connect/accept");
  description.add_options()("port,p", port_opt, "port to connect/accept");

  return description;
}

CommandLineOptions::VariablesMapType CommandLineOptions::ParseVariablesMap(
    int argc, char const* argv[]) const {
  namespace po = boost::program_options;

  auto vm = po::variables_map();
  po::store(po::parse_command_line(argc, argv, description_), vm);
  po::notify(vm);

  return vm;
}
}  // namespace ese