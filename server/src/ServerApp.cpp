#include "ServerApp.hpp"

#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>  // close, read

#include <cstdint>
#include <cstdio>   //std::perror
#include <cstdlib>  //std::exit, std::atoi
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "builders/ConcreteNetworkCreator.hpp"

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
Server::Server() : networkCreator{std::make_unique<ConcreteNetworkCreator>()} {
  std::cout << "Server started" << std::endl;
}

void Server::run(types::IP ip, types::Port port) {
  listenSocket = networkCreator->createSocket(ip, port);
  epoll = networkCreator->createEpoll();

  registerFD(listenSocket);
  receiveLoop(listenSocket);

  close(listenSocket);
  close(epoll);
  return;
}

void Server::registerFD(int fd) {
  epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;

  if (epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event) == -1) {
    std::perror("epoll_ctl() failed");
    close(fd);
    std::exit(EXIT_FAILURE);
  }
}

void Server::receiveLoop(int serverFD) {
  const int MAX_EVENTS = 10;
  std::vector<epoll_event> events(MAX_EVENTS);
  while (true) {
    int nfds = epoll_wait(epoll, events.data(), MAX_EVENTS, -1);

    if (nfds == -1) {
      std::perror("epoll_wait() failed");
      close(serverFD);
      close(epoll);
      std::exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nfds; i++) {
      int fd = events[i].data.fd;
      if (fd == serverFD) {
        handleNewConnection(serverFD, epoll);
      } else {
        handleClient(fd);
      }
    }
  }
}

void Server::handleNewConnection(int serverFD, int epoll) {
  sockaddr_in clientAdress{};
  socklen_t clientAddressLen = sizeof(clientAdress);

  int clientFD = accept4(serverFD, reinterpret_cast<sockaddr*>(&clientAdress),
                         &clientAddressLen, SOCK_NONBLOCK);

  if (clientFD == -1) {  // add EAGAIN or EWOULDBLOCK handling
    std::perror("accept4() failed");
    close(serverFD);
    close(epoll);
    std::exit(EXIT_FAILURE);
  }

  registerFD(clientFD);
}

void Server::handleClient(int fd) {
  std::vector<char> buffer(1024, '\0');
  ssize_t readBytes = recv(fd, buffer.data(), buffer.size(), MSG_DONTWAIT);

  if (readBytes == -1) {
    if (readBytes == EAGAIN or readBytes == EWOULDBLOCK) {
      std::cout << "No messages are available at the socket" << std::endl;
      return;
    } else {
      std::perror("recv() failed");
      close(fd);
      std::exit(EXIT_FAILURE);
    }
  } else if (readBytes == 0) {
    std::cout << "Connection closed by peer" << std::endl;
    return;
  } else {
    broadcast();
  }
}

void Server::broadcast() {
  // todo
}
Server::~Server() { std::cout << "Server shutdown" << std::endl; };
}  // namespace app