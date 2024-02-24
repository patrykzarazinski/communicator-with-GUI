#pragma once

#include <memory>
#include <vector>

#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class BaseSocket;
}  // namespace network

namespace app {
class Epoll;
class Server {
 public:
  Server();
  virtual ~Server();

  void run(types::IP, types::Port);

 private:
  void receiveLoop();
  void handleNewConnection();
  void handleClient(types::FD);
  void broadcast(std::vector<char>, types::FD);

  std::unique_ptr<network::BaseSocket> socket;
  std::unique_ptr<Epoll> epoll;
  bool serverIsRunning;
  std::vector<types::FD> clientsSocket;
};
}  // namespace app