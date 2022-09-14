#pragma once
#include <mutex>

#include "ClientContext.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

namespace client {
class ClientApp {
 public:
  ClientApp(types::IP, types::Port);
  virtual ~ClientApp();
  void run();

 private:
  void receiveLoop();
  void sender();

  ClientContext clientContext;
  std::mutex mutex;
};
}  // namespace client