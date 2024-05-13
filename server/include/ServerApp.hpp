#pragma once

#include <memory>
#include <vector>

#include "messages/Message.hpp"
#include "types/FD.hpp"
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
  void broadcast(const messages::Message&, const types::FD&);

  std::unique_ptr<network::ISocket> listenSocket;
  std::unique_ptr<Epoll> epoll;
  std::unique_ptr<core::Receiver> receiver;
  std::unique_ptr<core::Sender> sender;

  std::vector<types::FD> sockets;
};
}  // namespace app