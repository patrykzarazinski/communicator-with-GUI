#include "ServerApp.hpp"

#include <sys/socket.h>

#include <cstdint>
#include <cstdio>   //std::perror
#include <cstdlib>  //std::exit, std::atoi
#include <iostream>
#include <string>
#include <utility>
#include <vector>
//#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>  //inet_pton, htons
#include <sys/epoll.h>
#include <unistd.h>  // close, read

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
Server::Server() { std::cout << "Server started" << std::endl; }

void Server::run(types::IP ip, types::Port port) {
  int listener = socket(AF_INET, SOCK_STREAM,
                        0);  // int socket(int domain, int type, int protocol);
  if (listener == -1) {
    std::perror("socket() failed");
    std::exit(EXIT_FAILURE);
  }

  sockaddr_in serverAdress{};
  serverAdress.sin_family = AF_INET;
  serverAdress.sin_port = htons(static_cast<uint16_t>(std::atoi(port.c_str())));
  inet_pton(AF_INET, ip.c_str(), &serverAdress.sin_addr);

  if (bind(listener, reinterpret_cast<sockaddr*>(&serverAdress),
           sizeof(serverAdress)) == -1) {
    std::perror("bind() failed");
    close(listener);
    std::exit(EXIT_FAILURE);
  }
  int backlog = 10;  // maksymalna ilosc kolejki w oczekiwaniu na polaczenie,
                     // zastanow sie nad tym pozniej
  if (listen(listener, backlog) == -1) {
    std::perror("listen() failed");
    close(listener);
    std::exit(EXIT_FAILURE);
  }

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