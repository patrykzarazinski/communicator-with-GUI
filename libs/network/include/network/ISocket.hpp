#pragma once

#include <memory>

#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

struct sockaddr_in;

namespace network {
class ISocket {
 public:
  ISocket() = default;
  virtual ~ISocket() = default;

  virtual void createSocket(types::IP, types::Port) = 0;
  virtual types::FD getFD();
  virtual types::FD accept();

 protected:
  types::FD s_socket();
  types::FD s_accept4();
  void s_bind(sockaddr_in&, types::FD&);
  void s_listen(types::FD&);

  std::unique_ptr<types::FD> _socket{nullptr};
};
}  // namespace network