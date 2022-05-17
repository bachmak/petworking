#pragma once

#include "forwards.h"

namespace ese {

class Client {
 public:
  virtual void Start(std::function<void()> callback) = 0;
  virtual void SendMessage(std::string message,
                           std::function<void(std::string)> callback) = 0;
  virtual ~Client() = default;
};
}  // namespace ese