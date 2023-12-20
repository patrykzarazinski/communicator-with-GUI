#pragma once

#include "types/EpollFD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"
#include "types/SocketFD.hpp"

namespace app {
class NetworkCreator {
 public:
  virtual ~NetworkCreator() = default;
  virtual types::SocketFD createSocket(types::IP, types::Port) = 0;
  virtual types::EpollFD createEpoll() = 0;
};
}  // namespace app