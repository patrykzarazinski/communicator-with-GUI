#pragma once

#include "network/ISocket.hpp"
#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class ListenSocket final : public ISocket {
 public:
  ListenSocket() = default;
  virtual ~ListenSocket();

  void createSocket(types::IP, types::Port) override;
  types::FD accept() override;
};
}  // namespace network