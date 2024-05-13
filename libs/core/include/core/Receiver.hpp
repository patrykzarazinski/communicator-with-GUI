#pragma once

#include "messages/Message.hpp"
#include "types/FD.hpp"

namespace core {
class Receiver {
 public:
  messages::Message receive(const types::FD);
};
}  // namespace core