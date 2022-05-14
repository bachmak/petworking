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

  void ReadLine(StreamBuf& buf) {
    auto str = std::string();
    std::getline(is_, str);
    auto os = std::ostream(&buf);
    os << str;
  }

  template <typename It>
  void ReadLine(It buf_begin) {
    auto str = std::string();
    std::getline(is_, str);
    std::ranges::copy(str, buf_begin);
  }

  void Read(auto&&... args) { (is_ >> ... >> args); }

 private:
  std::ostream& os_;
  std::istream& is_;
};
}  // namespace ese