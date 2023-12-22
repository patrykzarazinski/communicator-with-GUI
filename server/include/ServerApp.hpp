#pragma once

#include <memory>
#include <vector>

#include "types/EpollFD.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"
#include "types/SocketFD.hpp"

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
  void handleClient(types::SocketFD);
  void broadcast(std::vector<char>, types::SocketFD);

  struct EpollManager {
    void registerSocket(types::SocketFD);
    types::EpollFD epoll;
  };

  std::unique_ptr<NetworkCreator> networkCreator;
  std::unique_ptr<EpollManager> epollManager;
  types::SocketFD serverSocket;
  bool serverIsRunning;
  std::vector<types::SocketFD> clientsSocket;
};
}  // namespace app