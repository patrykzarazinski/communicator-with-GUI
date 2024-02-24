#pragma once

#include "network/BaseSocket.hpp"
#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class ListenSocket : public BaseSocket {
 public:
  ListenSocket() = default;
  virtual ~ListenSocket();

  void createSocket(types::IP, types::Port) override;
  void receive() override;
  types::FD getFD() override;
};
}  // namespace network