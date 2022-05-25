#pragma once

#include "common/utils.h"

#define ESE_LOG_EC(logger, ec) \
  logger.LogLine("error:", ec.message(), "from func", BOOST_CURRENT_FUNCTION);

namespace ese {

class Logger {
 public:
  explicit Logger(std::ostream& os) : os_(os) {}

  void LogLine(auto&&... args) {
    Log(ESE_FWD(args));
    os_ << "\n";
  }

  void Log(auto&&... args) {
    os_ << utils::GetCurrentTime();
    ((os_ << " " << args), ...);
  }

 private:
  std::ostream& os_;
};
}  // namespace ese