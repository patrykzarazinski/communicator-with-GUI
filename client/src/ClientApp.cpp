#include "ClientApp.hpp"

#include <iostream>
#include <thread>

namespace {
void clearBuffer(std::string& buffer) {
  // TODO Using std::string::clear or std::string::erase lead to infinity loop
  // probably due to invalidate internal iterators, references, pointers
  for (auto& item : buffer) {
    item = '\0';
  }
}
}  // namespace
namespace client {
ClientApp::ClientApp(types::IP ip, types::Port port) : clientContext{ip, port} {
  std::cout << "Client started" << std::endl;
}

void ClientApp::run() {
  try {
    clientContext.socket.open(boost::asio::ip::tcp::v4());
    clientContext.socket.connect(clientContext.endpoint);
    std::thread receiver(&client::ClientApp::receiveLoop, this);
    std::thread sender(&client::ClientApp::sender, this);
    receiver.join();
    sender.join();
  } catch (const boost::system::system_error&) {
    // todo handler for exception
  }
}

void ClientApp::receiveLoop() {
  std::string buffer(512, '\0');
  while (true)  // TODO must somehow exit from this loop
  {
    clearBuffer(buffer);
    clientContext.socket.receive(boost::asio::buffer(buffer));
    // TODO change way of printing messages
    std::cout << buffer << std::endl;
  }
}

void ClientApp::sender() {
  std::string buffer;
  while (true)  // TODO must somehow exit from this loop
  {
    clearBuffer(buffer);
    std::getline(std::cin, buffer);
    clientContext.socket.send(boost::asio::buffer(buffer));
  }
}

ClientApp::~ClientApp() { std::cout << "Client shutdown" << std::endl; };
}  // namespace client