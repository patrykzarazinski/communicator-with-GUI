#include "TCPSocketCreator.hpp"

#include "TCPSocket.hpp"

namespace app {
TCPSocketCreator::TCPSocketCreator() {}

std::unique_ptr<Socket> TCPSocketCreator::create() {
  return std::make_unique<TCPSocket>();
}

TCPSocketCreator::~TCPSocketCreator() {}
}  // namespace app