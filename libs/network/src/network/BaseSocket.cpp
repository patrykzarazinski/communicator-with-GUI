#include "network/BaseSocket.hpp"

#include <netinet/in.h>  //sockaddr_in
#include <sys/socket.h>
#include <unistd.h>  // close, read

#include <cstdio>   //std::perror
#include <cstdlib>  //std::exit, std::atoi

namespace {
constexpr int PROTOCOL = 0;
constexpr int BACKLOG = 10;  // maksymalna ilosc kolejki w oczekiwaniu na
                             // polaczenie, zastanow sie nad tym pozniej
}  // namespace

namespace network {
void BaseSocket::send() {}

void BaseSocket::receive() {}

types::FD BaseSocket::b_socket() {
  types::FD fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);
  if (fd == -1) {
    std::perror("socket() failed");
    std::exit(EXIT_FAILURE);
  }
  return fd;
}

void BaseSocket::b_bind(sockaddr_in &adress, types::FD &fd) {
  if (bind(fd, reinterpret_cast<sockaddr *>(&adress), sizeof(adress)) == -1) {
    std::perror("bind() failed");
    close(fd);
    std::exit(EXIT_FAILURE);
  }
}

void BaseSocket::b_listen(types::FD &fd) {
  if (listen(fd, BACKLOG) == -1) {
    std::perror("listen() failed");
    close(fd);
    std::exit(EXIT_FAILURE);
  }
}
}  // namespace network