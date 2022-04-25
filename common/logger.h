#pragma once

#include "utils.h"

#define ESE_LOG_EC(logger, ec) \
  logger.LogLine("error:", ec.message(), "from func", BOOST_CURRENT_FUNCTION);

namespace ese {

class Logger {
 public:
  explicit Logger(std::ostream& os, std::istream& is) : os_(os), is_(is) {}

  void LogLine(auto&&... args) {
    Log(ESE_FWD(args));
    os_ << "\n";
  }

  void Log(auto&&... args) {
    os_ << GetCurrentTime();
    ((os_ << " " << args), ...);
  }

  void ReadLine(std::string& line) { std::getline(is_, line); }

  void Read(auto&&... args) { (is_ >> ... >> args); }

 private:
  std::ostream& os_;
  std::istream& is_;
};
}  // namespace ese