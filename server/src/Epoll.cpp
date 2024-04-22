#include "Epoll.hpp"

#include <sys/epoll.h>
#include <unistd.h>  // close, read

#include <vector>

#include "utils/ErrorHandler.hpp"

namespace {
const int MAX_EVENTS = 10;
const int BLOCK = -1;
}  // namespace

static std::vector<epoll_event> events(MAX_EVENTS);

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

types::NFDS Epoll::wait_for_event() {
  return epoll_wait(*_epoll, events.data(), MAX_EVENTS, BLOCK);
}

types::FD Epoll::getFD(int& i) { return events[i].data.fd; }

types::FD Epoll::getEpoll() { return *_epoll; }

Epoll::~Epoll() {
  if (_epoll) {
    close(*_epoll);
  }
}
}  // namespace app