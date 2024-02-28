#include "network/ISocket.hpp"

#include <netinet/in.h>  //sockaddr_in
#include <sys/socket.h>
#include <unistd.h>  // close, read

#include "utils/ErrorHandler.hpp"

namespace {
constexpr int PROTOCOL = 0;
constexpr int BACKLOG = 10;  // maksymalna ilosc kolejki w oczekiwaniu na
                             // polaczenie, zastanow sie nad tym pozniej
}  // namespace

namespace network {
types::FD ISocket::getFD() { return *_socket; }

types::FD ISocket::accept() { return -1; }

types::FD ISocket::s_socket() {
  types::FD fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);
  if (fd == -1) {
    utils::ErrorHandler::handleError("socket() failed");
  }
  return fd;
}

types::FD ISocket::s_accept4() {
  sockaddr_in address{};
  socklen_t addressLen = sizeof(address);

  return accept4(*_socket, reinterpret_cast<sockaddr *>(&address), &addressLen,
                 SOCK_NONBLOCK);
}

void ISocket::s_bind(sockaddr_in &adress, types::FD &fd) {
  if (bind(fd, reinterpret_cast<sockaddr *>(&adress), sizeof(adress)) == -1) {
    close(fd);
    utils::ErrorHandler::handleError("bind() failed");
  }
}

void ISocket::s_listen(types::FD &fd) {
  if (listen(fd, BACKLOG) == -1) {
    close(fd);
    utils::ErrorHandler::handleError("listen() failed");
  }
}
}  // namespace network