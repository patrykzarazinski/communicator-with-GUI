#pragma once

#include "Socket.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace app {
class TCPSocket : public Socket {
 public:
  TCPSocket();
  virtual ~TCPSocket();

  int createSocket(types::IP, types::Port) override;

 private:
  types::IP ip;
  types::Port port;
};
}  // namespace app