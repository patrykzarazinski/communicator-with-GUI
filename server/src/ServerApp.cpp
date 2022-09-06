#include "ServerApp.hpp"

#include <cstdint>
#include <thread>
#include <utility>

namespace {
types::ClientID generateClientID() {
  // todo implement
  return 1;
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
}  // namespace
namespace server {
ServerApp::ServerApp(types::IP ip, types::Port port)
    : serverContext{ip, port} {}

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
    boost::asio::ip::tcp::socket socket{serverContext.socket.accept()};
    const auto clientID = generateClientID();
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

  std::string buffer;
  while (true)  // TODO think about some exit of infinity loop
  {
    socket.receive(boost::asio::buffer(buffer));
    broadcast(clientID, buffer);
  }
}

void ServerApp::broadcast(const types::ClientID& clientID, std::string& msg) {
  std::lock_guard<std::mutex> lock(mutex);
  for (auto& [id, socket] : clients) {
    if (id != clientID) {
      socket.send(boost::asio::buffer(msg));
    }
  }
}

ServerApp::~ServerApp() = default;
}  // namespace server