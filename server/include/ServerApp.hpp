#pragma once

#include <memory>

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
  void registerFD(int);
  void receiveLoop(int);
  void handleNewConnection(int, int);
  void handleClient(int);
  void broadcast();

  std::unique_ptr<NetworkCreator> networkCreator;
  types::SocketFD listenSocket;
  types::EpollFD epoll;
};
}  // namespace app