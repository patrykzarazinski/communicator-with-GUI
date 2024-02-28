#pragma once

#include <memory>
#include <vector>

#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class ISocket;
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

  std::unique_ptr<network::ISocket> listenSocket;
  std::unique_ptr<Epoll> epoll;
  std::vector<types::FD> sockets;
};
}  // namespace app