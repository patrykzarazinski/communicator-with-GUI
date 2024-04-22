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
#include "network/ListenSocket.hpp"
#include "utils/ErrorHandler.hpp"

static bool isRunning = false;

namespace {
void clearBuffer(std::string& buffer) {
  // TODO Using std::string::clear or std::string::erase lead to infinity loop
  // probably due to invalidate internal iterators, references, pointers
  for (auto& item : buffer) {
    item = '\0';
  }
}

}  // namespace
namespace app {
Server::Server()
    : listenSocket{std::make_unique<network::ListenSocket>()},
      epoll{std::make_unique<Epoll>()} {}

void Server::run(types::IP ip, types::Port port) {
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

  sockets.push_back(socket);
  epoll->registerSocket(socket);
}

void Server::handleClient(types::FD socket) {
  std::vector<char> buffer(1024, '\0');
  ssize_t readBytes = recv(socket, buffer.data(), buffer.size(), MSG_DONTWAIT);

  if (readBytes == -1) {
    if (readBytes == EAGAIN or readBytes == EWOULDBLOCK) {
      std::cout << "No messages are available at the socket" << std::endl;
      return;
    } else {
      close(socket);
      utils::ErrorHandler::handleError("recv() failed");
    }
  } else if (readBytes == 0) {
    std::cout << "Connection closed by peer" << std::endl;
    close(socket);
    return;
  } else {
    broadcast(buffer, socket);
  }
}

void Server::broadcast(std::vector<char> buffer, types::FD socket) {
  for (const types::FD& fd : sockets) {
    if (fd != socket) {
      send(fd, buffer.data(), buffer.size(), 0);
    }
  }
}

Server::~Server() { std::cout << "Server shutdown" << std::endl; };
}  // namespace app