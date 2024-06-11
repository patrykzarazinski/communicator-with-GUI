#pragma once

#include <memory>
#include <string>

#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class ISocket;
}  // namespace network

namespace core {
class Sender;
}  // namespace core

namespace app {
class CLI {
 public:
  CLI();
  virtual ~CLI();

  void run(types::IP, types::Port);

 private:
  void loop();
  void handleCommand(const std::string&);

  std::unique_ptr<network::ISocket> socket;
  std::unique_ptr<core::Sender> sender;
};
}  // namespace app