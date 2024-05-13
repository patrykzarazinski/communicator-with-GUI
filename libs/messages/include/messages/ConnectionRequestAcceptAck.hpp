#pragma once

#include "types/TransactionId.hpp"
#include "types/UUID.hpp"

namespace messages {
struct ConnectionRequestAcceptAck {
  types::TransactionId transactionId;
  types::UUID uuid;
};
}  // namespace messages