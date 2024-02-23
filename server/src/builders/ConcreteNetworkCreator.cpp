#include "builders/ConcreteNetworkCreator.hpp"

#include <arpa/inet.h>   //inet_pton, htons
#include <netinet/in.h>  //sockaddr_in
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>  // close, read

#include <cstdio>   //std::perror
#include <cstdlib>  //std::exit, std::atoi

namespace {
constexpr int PROTOCOL = 0;
constexpr int BACKLOG = 10;  // maksymalna ilosc kolejki w oczekiwaniu na
                             // polaczenie, zastanow sie nad tym pozniej
}  // namespace

namespace app {
ConcreteNetworkCreator::ConcreteNetworkCreator() {}

types::FD ConcreteNetworkCreator::createSocket(types::IP ip,
                                                     types::Port port) {
  int fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);
  if (fd == -1) {
    std::perror("socket() failed");
    std::exit(EXIT_FAILURE);
  }

  sockaddr_in serverAdress{};
  serverAdress.sin_family = AF_INET;
  serverAdress.sin_port = htons(static_cast<uint16_t>(std::atoi(port.c_str())));
  inet_pton(AF_INET, ip.c_str(), &serverAdress.sin_addr);

  if (bind(fd, reinterpret_cast<sockaddr*>(&serverAdress),
           sizeof(serverAdress)) == -1) {
    std::perror("bind() failed");
    close(fd);
    std::exit(EXIT_FAILURE);
  }

  if (listen(fd, BACKLOG) == -1) {
    std::perror("listen() failed");
    close(fd);
    std::exit(EXIT_FAILURE);
  }

  return static_cast<types::FD>(fd);
}
ConcreteNetworkCreator::~ConcreteNetworkCreator() {}
}  // namespace app