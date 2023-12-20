#pragma once

#include "NetworkCreator.hpp"

namespace app {
class ConcreteNetworkCreator : public NetworkCreator {
 public:
  ConcreteNetworkCreator();
  virtual ~ConcreteNetworkCreator();

  types::SocketFD createSocket(types::IP, types::Port) override;
  types::EpollFD createEpoll() override;
};
}  // namespace app