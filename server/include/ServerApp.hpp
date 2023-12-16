#pragma once

#include <mutex>

#include "types/IP.hpp"
#include "types/Port.hpp"

namespace app {
class Server {
 public:
  Server();
  virtual ~Server();

  void run(types::IP, types::Port);

 private:
  void handleNewConnection(int, int);
  void handleClient(int);
  void broadcast();

  std::mutex mutex;
};
}  // namespace app