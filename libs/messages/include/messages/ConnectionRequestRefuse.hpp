#pragma once

#include "types/Cause.hpp"
#include "types/TransactionId.hpp"

namespace messages {
struct ConnectionRequestRefuse {
  types::TransactionId transactionId;
  types::Cause cause;
};
}  // namespace messages