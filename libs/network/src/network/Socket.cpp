#include "network/Socket.hpp"

#include <arpa/inet.h>   //inet_pton, htons
#include <netinet/in.h>  //sockaddr_in

#include <cstdlib>  //std::exit, std::atoi

namespace network {
void Socket::createSocket(types::IP ip, types::Port port) {
  if (_socket) {
    return;
  }

  types::FD fd = s_socket();

  sockaddr_in adress{};
  adress.sin_family = AF_INET;
  adress.sin_port = htons(static_cast<uint16_t>(std::atoi(port.c_str())));
  inet_pton(AF_INET, ip.c_str(), &adress.sin_addr);

  s_bind(adress, fd);

  _socket = std::make_unique<types::FD>(fd);
}

Socket::~Socket() {
  if (_socket) {
    close(*_socket);
  }
}
}  // namespace network