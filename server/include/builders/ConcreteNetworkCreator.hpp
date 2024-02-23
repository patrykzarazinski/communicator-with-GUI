#pragma once

#include "NetworkCreator.hpp"

namespace app {
class ConcreteNetworkCreator : public NetworkCreator {
 public:
  ConcreteNetworkCreator();
  virtual ~ConcreteNetworkCreator();

  types::FD createSocket(types::IP, types::Port) override;
};
}  // namespace app