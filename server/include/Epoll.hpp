#pragma once

#include <memory>

#include "types/FD.hpp"
#include "types/NFDS.hpp"

namespace app {
class Epoll final {
 public:
  Epoll() = default;
  ~Epoll();

  void createEpoll();
  void registerSocket(types::FD);
  types::NFDS wait_for_event();
  types::FD getFD(int&);
  types::FD getEpoll();

 private:
  std::unique_ptr<types::FD> _epoll;
};
}  // namespace app