#pragma once

#include <memory>
#include <vector>

#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace network {
class BaseSocket;
}

namespace app {
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

  struct EpollManager {
    void registerSocket(types::FD);
    types::FD createEpoll();
    types::FD epoll;
  };

  std::unique_ptr<network::BaseSocket> socket;
  std::unique_ptr<EpollManager> epollManager;
  bool serverIsRunning;
  std::vector<types::FD> clientsSocket;
};
}  // namespace app