#include "Epoll.hpp"

#include <sys/epoll.h>
#include <unistd.h>  // close, read

#include "utils/ErrorHandler.hpp"

namespace app {
void Epoll::createEpoll() {
  if (_epoll) {
    return;
  }

  int ignoredFlag = 0;
  types::FD fd = static_cast<types::FD>(epoll_create1(ignoredFlag));

  if (fd == -1) {
    utils::ErrorHandler::handleError("epoll_create1() failed");
  }

  _epoll = std::make_unique<types::FD>(fd);
}

void Epoll::registerSocket(types::FD fd) {
  epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;

  if (epoll_ctl(*_epoll, EPOLL_CTL_ADD, fd, &event) == -1) {
    close(fd);
    utils::ErrorHandler::handleError("epoll_ctl(() failed");
  }
}

types::FD Epoll::getEpoll() { return *_epoll; }

Epoll::~Epoll() {
  if (_epoll) {
    close(*_epoll);
  }
}
}  // namespace app