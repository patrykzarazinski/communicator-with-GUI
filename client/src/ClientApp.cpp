#include "ClientApp.hpp"

#include <iostream>
#include <thread>

#include "core/Receiver.hpp"
#include "core/Sender.hpp"
#include "messages/Message.hpp"
#include "network/Socket.hpp"
#include "utils/Overload.hpp"

namespace {
void clearBuffer(std::string& buffer) {
  // TODO Using std::string::clear or std::string::erase lead to infinity loop
  // probably due to invalidate internal iterators, references, pointers
  for (auto& item : buffer) {
    item = '\0';
  }
}
}  // namespace
namespace app {
Client::Client()
    : socket{std::make_unique<network::Socket>()},
      receiver{std::make_unique<core::Receiver>()},
      sender{std::make_unique<core::Sender>()} {}

void Client::run(types::IP ip, types::Port port) {
  socket->createSocket(ip, port);

  std::thread receiving(&app::Client::receiveLoop, this);
  std::thread sending(&app::Client::senderLoop, this);

  receiving.join();
  sending.join();
}

void Client::receiveLoop() {
  while (true)  // TODO must somehow exit from this loop
  {
    messages::Message message = receiver->receive(socket->getFD());

    // TODO not all messages belong to client app
    std::visit(utils::overload{
                   [](messages::Data& msg) {
                     // TODO add loger lib
                     std::cout << msg.data << std::endl;
                   },
                   [](messages::ConnectionRequest& msg) {
                     std::cout << "Message ConnectionRequest received"
                               << std::endl;
                   },
                   [](messages::ConnectionRequestAccept& msg) {
                     std::cout << "Message ConnectionRequestAccept received"
                               << std::endl;
                   },
                   [](messages::ConnectionRequestAcceptAck& msg) {
                     std::cout << "Message ConnectionRequestAcceptAck received"
                               << std::endl;
                   },
                   [](messages::ConnectionRequestRefuse& msg) {
                     std::cout << "Message ConnectionRequestRefuse received"
                               << std::endl;
                   },
                   [](messages::ConnectionDisconnection& msg) {
                     std::cout << "Message ConnectionDisconnection received"
                               << std::endl;
                   }},
               message);
  }
}

void Client::senderLoop() {
  while (true)  // TODO must somehow exit from this loop
  {
    messages::Message message = messages::Data();
    std::getline(std::cin, std::get<messages::Data>(message).data);
    sender->send(socket->getFD(), message);
  }
}

Client::~Client() { std::cout << "Client shutdown" << std::endl; };
}  // namespace app