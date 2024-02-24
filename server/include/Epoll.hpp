#pragma once

#include <memory>

#include "types/FD.hpp"

namespace app {
class Epoll {
 public:
  Epoll() = default;
  ~Epoll();
  void createEpoll();
  void registerSocket(types::FD);
  types::FD getEpoll();

 private:
  std::unique_ptr<types::FD> _epoll;
};
}  // namespace app