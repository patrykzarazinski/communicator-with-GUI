#pragma once

#include "types/TransactionId.hpp"
#include "types/UUID.hpp"

namespace messages {
struct ConnectionRequestAccept {
  types::TransactionId transactionId;
  types::UUID uuid;
};
}  // namespace messages