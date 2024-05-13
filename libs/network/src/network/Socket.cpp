#include "network/Socket.hpp"

#include <arpa/inet.h>   //inet_pton, htons
#include <netinet/in.h>  //sockaddr_in

#include <cstdlib>  //std::exit, std::atoi

#include "utils/ErrorHandler.hpp"

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

  s_connect(adress, fd);

  _socket = std::make_unique<types::FD>(fd);
}

void Socket::s_connect(sockaddr_in &adress, types::FD &fd) {
  if (connect(fd, (struct sockaddr *)&adress, sizeof(adress)) < 0) {
    utils::ErrorHandler::handleError("Error with connect function");
  }
}

Socket::~Socket() {
  if (_socket) {
    close(*_socket);
  }
}
}  // namespace network