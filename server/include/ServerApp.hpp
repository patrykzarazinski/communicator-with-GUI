#pragma once

#include <memory>

#include "SocketCreator.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace app {
class Server {
 public:
  Server();
  virtual ~Server();

  void run(types::IP, types::Port);

 private:
  void handleNewConnection(int, int);
  void handleClient(int);
  void broadcast();

  std::unique_ptr<SocketCreator> socketCreator;
};
}  // namespace app