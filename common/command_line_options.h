#pragma once

#include "forwards.h"

namespace ese {

class CommandLineOptions {
 private:
  using DescriptionType = boost::program_options::options_description;
  using VariablesMapType = boost::program_options::variables_map;

 public:
  explicit CommandLineOptions(int argc, char const* argv[]);

 public:
  friend std::ostream& operator<<(std::ostream& os,
                                  const CommandLineOptions& options);

  bool Help() const;

  const std::string& Protocol() const;

  const std::string& Port() const;

  const std::string& Host() const;

 private:
  static DescriptionType CreateDescription();

  VariablesMapType ParseVariablesMap(int argc, char const* argv[]) const;

 private:
  DescriptionType description_;
  VariablesMapType variables_map_;
};
}  // namespace ese
