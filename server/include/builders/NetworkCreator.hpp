#pragma once

#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"
#include "types/FD.hpp"

namespace app {
class NetworkCreator {
 public:
  virtual ~NetworkCreator() = default;
  virtual types::FD createSocket(types::IP, types::Port) = 0;
};
}  // namespace app