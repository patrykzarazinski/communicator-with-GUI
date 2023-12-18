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

#include "TCPSocket.hpp"
#include "TCPSocketCreator.hpp"

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
Server::Server() : socketCreator{std::make_unique<TCPSocketCreator>()} {
  std::cout << "Server started" << std::endl;
}

void Server::run(types::IP ip, types::Port port) {
  std::unique_ptr<Socket> socket = socketCreator->create();
  int listener = socket->createSocket(ip, port);

  // epoll
  int epoll = epoll_create1(0);
  if (epoll == -1) {
    std::perror("epoll_create1() failed");
    close(listener);
    std::exit(EXIT_FAILURE);
  }

  epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = listener;

  if (epoll_ctl(epoll, EPOLL_CTL_ADD, listener, &event) == -1) {
    std::perror("epoll_ctl() failed");
    close(listener);
    std::exit(EXIT_FAILURE);
  }

  const int MAX_EVENTS = 10;
  std::vector<epoll_event> events(MAX_EVENTS);
  while (true) {
    int nfds = epoll_wait(epoll, events.data(), MAX_EVENTS, -1);

    if (nfds == -1) {
      std::perror("epoll_wait() failed");
      close(listener);
      close(epoll);
      std::exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nfds; i++) {
      int fd = events[i].data.fd;
      if (fd == listener) {
        handleNewConnection(listener, epoll);
      } else {
        handleClient(fd);
      }
    }
  }

  close(listener);
  close(epoll);
  return;
}

void Server::handleNewConnection(int listener, int epoll) {
  sockaddr_in clientAdress{};
  socklen_t clientAddressLen = sizeof(clientAdress);

  int newFd = accept4(listener, reinterpret_cast<sockaddr*>(&clientAdress),
                      &clientAddressLen, SOCK_NONBLOCK);

  if (newFd == -1) {  // add EAGAIN or EWOULDBLOCK handling
    std::perror("accept4() failed");
    close(listener);
    close(epoll);
    std::exit(EXIT_FAILURE);
  }

  epoll_event event;
  event.events = EPOLLIN;  // EPOLLET?
  event.data.fd = newFd;

  if (epoll_ctl(epoll, EPOLL_CTL_ADD, newFd, &event) == -1) {
    std::perror("epoll_ctl() failed");
    close(listener);
    close(epoll);
    std::exit(EXIT_FAILURE);
  }
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