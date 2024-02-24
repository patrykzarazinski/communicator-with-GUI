#include "Epoll.hpp"

#include <sys/epoll.h>
#include <unistd.h>  // close, read

#include <cstdio>   //std::perror
#include <cstdlib>  //std::exit, std::atoi

namespace app {
void Epoll::createEpoll() {
  if (_epoll) {
    return;
  }

  int ignoredFlag = 0;
  types::FD fd = static_cast<types::FD>(epoll_create1(ignoredFlag));

  if (fd == -1) {
    std::perror("epoll_create1() failed");
    std::exit(EXIT_FAILURE);
  }

  _epoll = std::make_unique<types::FD>(fd);
}

void Epoll::registerSocket(types::FD fd) {
  epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;

  if (epoll_ctl(*_epoll, EPOLL_CTL_ADD, fd, &event) == -1) {
    std::perror("epoll_ctl() failed");
    close(fd);
    std::exit(EXIT_FAILURE);
  }
}

types::FD Epoll::getEpoll() { return *_epoll; }

Epoll::~Epoll() {
  if (_epoll) {
    close(*_epoll);
  }
}
}  // namespace app