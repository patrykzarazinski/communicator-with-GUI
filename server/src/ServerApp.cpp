#include "ServerApp.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>  // close, read

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Epoll.hpp"
#include "core/Receiver.hpp"
#include "core/Sender.hpp"
#include "network/ListenSocket.hpp"
//#include "spdlog/spdlog.h"
#include "utils/ErrorHandler.hpp"
#include "utils/Overload.hpp"

static bool isRunning = false;

namespace app {
Server::Server()
    : listenSocket{std::make_unique<network::ListenSocket>()},
      epoll{std::make_unique<Epoll>()},
      receiver{std::make_unique<core::Receiver>()},
      sender{std::make_unique<core::Sender>()} {}

void Server::run(types::IP ip, types::Port port) {
 // spdlog::info("Server started");

  listenSocket->createSocket(ip, port);
  epoll->createEpoll();

  epoll->registerSocket(listenSocket->getFD());
  receiveLoop();

  return;
}

void Server::receiveLoop() {
  isRunning = true;
  while (isRunning) {
    types::NFDS nfds = epoll->wait_for_event();

    if (nfds == -1) {
      utils::ErrorHandler::handleError("wait_for_event|epoll_wait() failed");
    }

    for (int i = 0; i < nfds; i++) {
      types::FD fd = epoll->getFD(i);
      if (fd == listenSocket->getFD()) {
        handleNewConnection();
      } else {
        handleClient(fd);
      }
    }
  }
}

void Server::handleNewConnection() {
  int socket = listenSocket->accept();

  if (socket == -1) {  // add EAGAIN or EWOULDBLOCK handling
    utils::ErrorHandler::handleError("accept4() failed");
  }
  /*
    TODO Aby moc przeprowadzic rejestracje uzytkownia i odebrac wiadomosci, recv
    musi blokowac doczasu otrzymania wiadomosci, aby to zrobic trzeba socketa
    ustawic na SOCK_STREAM i ewentuanie jakis timeout, i zmienic accept_4 na
    accept bez flagi, ktora daje errory jak jest socket na block ustawiony
  */
  sockets.push_back(socket);
  epoll->registerSocket(socket);
}

void Server::handleClient(types::FD socket) {
  messages::Message message = receiver->receive(socket);

  std::visit(
      utils::overload{[this, &socket](messages::Data& msg) {
                     //   spdlog::info("Data received");
                        broadcast(msg, socket);
                      },
                      [](messages::ConnectionRequest& msg) {
                    //    spdlog::info("ConnectionRequest received");
                      },
                      [](messages::ConnectionRequestAccept& msg) {
                     //   spdlog::info("ConnectionRequestAccept received");
                      },
                      [](messages::ConnectionRequestAcceptAck& msg) {
                    //    spdlog::info("ConnectionRequestAcceptAck received");
                      },
                      [](messages::ConnectionRequestRefuse& msg) {
                    //    spdlog::info("ConnectionRequestRefuse received");
                      },
                      [](messages::ConnectionDisconnection& msg) {
                    //    spdlog::info("ConnectionDisconnection received");
                      }},
      message);
}

void Server::broadcast(const messages::Message& message,
                       const types::FD& socket) {
  for (const types::FD& fd : sockets) {
    if (fd != socket) {
      sender->send(fd, message);
    }
  }
}

Server::~Server() {
  // spdlog::info("Server shutdown");
   };
}  // namespace app