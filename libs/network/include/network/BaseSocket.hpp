#pragma once

#include <memory>

#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

struct sockaddr_in;

namespace network {
class BaseSocket {
 public:
  BaseSocket() = default;
  virtual ~BaseSocket() = default;

  virtual void createSocket(types::IP, types::Port) = 0;
  virtual void send();
  virtual void receive();
  virtual types::FD getFD() = 0;

 protected:
  types::FD b_socket();
  void b_bind(sockaddr_in&, types::FD&);
  void b_listen(types::FD&);

  std::unique_ptr<types::FD> _socket{nullptr};
};
}  // namespace network