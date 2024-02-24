#include "network/ListenSocket.hpp"

#include <arpa/inet.h>   //inet_pton, htons
#include <netinet/in.h>  //sockaddr_in

namespace network {
void ListenSocket::createSocket(types::IP ip, types::Port port) {
  if (_socket) {
    return;
  }

  types::FD fd = b_socket();

  sockaddr_in adress{};
  adress.sin_family = AF_INET;
  adress.sin_port = htons(static_cast<uint16_t>(std::atoi(port.c_str())));
  inet_pton(AF_INET, ip.c_str(), &adress.sin_addr);

  b_bind(adress, fd);
  b_listen(fd);

  _socket = std::make_unique<types::FD>(fd);
}

void ListenSocket::receive() {
  // todo
}

types::FD ListenSocket::getFD() { return *_socket; }
}  // namespace network