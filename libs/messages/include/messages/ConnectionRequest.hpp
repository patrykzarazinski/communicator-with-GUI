#pragma once

#include "types/TransactionId.hpp"

namespace messages {
struct ConnectionRequest {
  types::TransactionId transactionId;
};
}  // namespace messages