#pragma once

#include <memory>
#include <vector>

#include "types/FD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"
#include "types/FD.hpp"

namespace app {
class NetworkCreator;
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

  std::unique_ptr<NetworkCreator> networkCreator;
  std::unique_ptr<EpollManager> epollManager;
  types::FD serverSocket;
  bool serverIsRunning;
  std::vector<types::FD> clientsSocket;
};
}  // namespace app