#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

#include "ServerContext.hpp"
#include "types/ClientID.hpp"

namespace server {
class ServerApp final {
 public:
  using Clients =
      std::unordered_map<types::ClientID, boost::asio::ip::tcp::socket>;
  using Client = Clients::iterator;
  ServerApp(types::IP, types::Port);
  virtual ~ServerApp();

  void run();

 private:
  void receiveLoop();
  void runClient(types::ClientID);
  void broadcast(const types::ClientID&, std::string&);

  ServerContext serverContext;
  Clients clients;
  std::mutex mutex;
};
}  // namespace server