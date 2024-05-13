#pragma once

#include "messages/Message.hpp"
#include "types/FD.hpp"

namespace core {
class Sender {
 public:
  void send(const types::FD, messages::Message);
};
}  // namespace core