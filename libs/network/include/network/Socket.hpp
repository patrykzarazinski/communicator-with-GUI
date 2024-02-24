#pragma once

#include "network/BaseSocket.hpp"
#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class Socket : public BaseSocket {
 public:
  Socket() = default;
  virtual ~Socket();

  void createSocket(types::IP, types::Port) override;
  void send() override;
  void receive() override;
  types::FD getFD() override;
};
}  // namespace network