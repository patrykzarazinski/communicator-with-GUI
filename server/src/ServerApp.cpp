#include "ServerApp.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include "boost/asio/basic_stream_socket.hpp"

namespace {
types::ClientID generateClientID(int& clientIdTemporary) {
  // todo implement
  clientIdTemporary++;
  return clientIdTemporary;
}
server::ServerApp::Client getClientData(std::mutex& mutex,
                                        const types::ClientID& clientID,
                                        server::ServerApp::Clients& clients) {
  std::lock_guard<std::mutex> lock(mutex);
  auto client = clients.find(clientID);
  if (client == clients.end()) {
    // TODO Log error
  }
  return client;
}
void clearBuffer(std::string& buffer) {
  // TODO Using std::string::clear or std::string::erase lead to infinity loop
  // probably due to invalidate internal iterators, references, pointers
  for (auto& item : buffer) {
    item = '\0';
  }
}
}  // namespace
namespace server {
ServerApp::ServerApp(types::IP ip, types::Port port)
    : serverContext{ip, port}, clientIdTemporary{0} {
  std::cout << "Server started" << std::endl;
}

void ServerApp::run() {
  try {
    serverContext.socket.open(boost::asio::ip::tcp::v4());
    serverContext.socket.bind(serverContext.endpoint);
    serverContext.socket.listen();
    receiveLoop();
  } catch (const boost::system::system_error&) {
    // TODO Log error
  }
}

void ServerApp::receiveLoop() {
  while (true)  // TODO think about some exit of infinity loop
  {
    std::cout << "Waiting for client to join..." << std::endl;
    boost::asio::ip::tcp::socket socket{serverContext.socket.accept()};
    const auto clientID = generateClientID(this->clientIdTemporary);
    std::cout << "Client with id: " << clientID << " joined" << std::endl;
    {
      std::lock_guard<std::mutex> lock(mutex);
      clients.emplace(clientID, std::move(socket));
    }
    std::thread thread(&server::ServerApp::runClient, this, clientID);
    thread.detach();
  }
}

void ServerApp::runClient(types::ClientID clientID) {
  Client client = getClientData(mutex, clientID, clients);
  auto& socket = client->second;

  std::string buffer(512, '\0');
  while (true)  // TODO think about some exit of infinity loop
  {
    clearBuffer(buffer);
    socket.receive(boost::asio::buffer(buffer));  // return how many received
    broadcast(clientID, buffer);
  }
}

void ServerApp::broadcast(const types::ClientID& clientID, std::string& msg) {
  std::lock_guard<std::mutex> lock(mutex);
  for (auto& [id, socket] : clients) {
    if (id != clientID) {
      socket.send(boost::asio::buffer(msg, msg.size()));  // return how many
                                                          // send
    }
  }
}

ServerApp::~ServerApp() { std::cout << "Server shutdown" << std::endl; };
}  // namespace server