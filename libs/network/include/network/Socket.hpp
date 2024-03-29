#pragma once

#include "network/ISocket.hpp"
#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class Socket final : public ISocket {
 public:
  Socket() = default;
  virtual ~Socket();

  void createSocket(types::IP, types::Port) override;
  // todo
};
}  // namespace network