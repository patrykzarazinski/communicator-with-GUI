#pragma once

#include <memory>

#include "Socket.hpp"

namespace app {
class SocketCreator {
 public:
  virtual ~SocketCreator() = default;
  virtual std::unique_ptr<Socket> create() = 0;
};
}  // namespace app