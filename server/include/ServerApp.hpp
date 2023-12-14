#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

#include "ServerContext.hpp"
#include "types/ClientID.hpp"

namespace app {
class Server {
 public:
  using Clients =
      std::unordered_map<types::ClientID, boost::asio::ip::tcp::socket>;
  using Client = Clients::iterator;

  Server();
  virtual ~Server();

  void run(types::IP, types::Port);

 private:
  void handleNewConnection();
  void handleClient();
  void runClient(types::ClientID);

  ServerContext serverContext;
  Clients clients;
  std::mutex mutex;
};
}  // namespace app