#include "network/Socket.hpp"

#include <arpa/inet.h>   //inet_pton, htons
#include <netinet/in.h>  //sockaddr_in

namespace network {
void Socket::createSocket(types::IP ip, types::Port port) {
  if (_socket) {
    return;
  }

  types::FD fd = b_socket();

  sockaddr_in adress{};
  adress.sin_family = AF_INET;
  adress.sin_port = htons(static_cast<uint16_t>(std::atoi(port.c_str())));
  inet_pton(AF_INET, ip.c_str(), &adress.sin_addr);

  b_bind(adress, fd);

  _socket = std::make_unique<types::FD>(fd);
}

void Socket::send() {
  // todo
}
void Socket::receive() {
  // todo
}

types::FD Socket::getFD() { return *_socket; }
}  // namespace network