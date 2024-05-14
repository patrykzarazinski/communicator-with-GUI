#include "ClientApp.hpp"

#include <csignal>
#include <iostream>
#include <thread>

#include "core/Receiver.hpp"
#include "core/Sender.hpp"
#include "messages/Message.hpp"
#include "network/Socket.hpp"
//#include "spdlog/spdlog.h"
#include "utils/Overload.hpp"

static bool isReceiveLoopRunning = false;
static bool isSenderLoopRunning = false;

namespace {
void signalHandler(int sig) {// spdlog::info("Better not to use ctrl+c"); 
}

}  // namespace
namespace app {
Client::Client()
    : socket{std::make_unique<network::Socket>()},
      receiver{std::make_unique<core::Receiver>()},
      sender{std::make_unique<core::Sender>()} {}

void Client::run(types::IP ip, types::Port port) {
  std::signal(SIGINT, signalHandler);

  socket->createSocket(ip, port);

  std::thread receiving(&app::Client::receiveLoop, this);
  std::thread sending(&app::Client::senderLoop, this);

  receiving.join();
  sending.join();
}

void Client::receiveLoop() {
  isReceiveLoopRunning = true;
  while (isReceiveLoopRunning)  // TODO must somehow exit from this loop
  {
    messages::Message message = receiver->receive(socket->getFD());

    // TODO not all messages belong to client app
    std::visit(
        utils::overload{[](messages::Data& msg) {
                          //spdlog::info("Data received");
                          std::cout << msg.data << std::endl;
                        },
                        [](messages::ConnectionRequest& msg) {
                          //spdlog::info("ConnectionRequest received");
                        },
                        [](messages::ConnectionRequestAccept& msg) {
                         // spdlog::info("ConnectionRequestAccept received");
                        },
                        [](messages::ConnectionRequestAcceptAck& msg) {
                         // spdlog::info("ConnectionRequestAcceptAck received");
                        },
                        [](messages::ConnectionRequestRefuse& msg) {
                        //  spdlog::info("ConnectionRequestRefuse received");
                        },
                        [](messages::ConnectionDisconnection& msg) {
                       //   spdlog::info("ConnectionDisconnection received");
                        }},
        message);
  }
}

void Client::senderLoop() {
  isSenderLoopRunning = true;
  while (isSenderLoopRunning)  // TODO must somehow exit from this loop
  {
    messages::Message message = messages::Data();
    std::getline(std::cin, std::get<messages::Data>(message).data);
    sender->send(socket->getFD(), message);
  }
}

Client::~Client() {// spdlog::info("Client shutdown"); 

};
}  // namespace app