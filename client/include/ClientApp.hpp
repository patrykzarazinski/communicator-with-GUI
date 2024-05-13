#pragma once

#include <memory>

#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class ISocket;
}  // namespace network

namespace core {
class Receiver;
class Sender;
}  // namespace core

namespace app {
class Client {
 public:
  Client();
  virtual ~Client();

  void run(types::IP, types::Port);

 private:
  void receiveLoop();
  void senderLoop();

  std::unique_ptr<network::ISocket> socket;
  std::unique_ptr<core::Receiver> receiver;
  std::unique_ptr<core::Sender> sender;
};
}  // namespace app