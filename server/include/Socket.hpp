#pragma once

#include "types/IP.hpp"
#include "types/Port.hpp"

namespace app {
class Socket {
 public:
  virtual ~Socket() = default;
  virtual int createSocket(types::IP, types::Port) = 0;
};
}  // namespace app