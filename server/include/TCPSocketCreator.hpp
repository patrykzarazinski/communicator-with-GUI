#pragma once

#include <memory>

#include "Socket.hpp"
#include "SocketCreator.hpp"

namespace app {
class TCPSocketCreator : public SocketCreator {
 public:
  TCPSocketCreator();
  virtual ~TCPSocketCreator();

  std::unique_ptr<Socket> create() override;
};
}  // namespace app