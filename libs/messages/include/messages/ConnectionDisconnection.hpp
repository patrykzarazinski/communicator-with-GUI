#pragma once

#include "types/UUID.hpp"

namespace messages {
struct ConnectionDisconnection {
  types::UUID uuid;
};
}  // namespace messages