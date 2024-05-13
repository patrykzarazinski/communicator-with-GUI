#pragma once

#include <variant>

#include "messages/ConnectionDisconnection.hpp"
#include "messages/ConnectionRequest.hpp"
#include "messages/ConnectionRequestAccept.hpp"
#include "messages/ConnectionRequestAcceptAck.hpp"
#include "messages/ConnectionRequestRefuse.hpp"
#include "messages/Data.hpp"

// clang-format off
namespace messages {
using Message =
    std::variant<
        messages::ConnectionDisconnection,
        messages::ConnectionRequest,
        messages::ConnectionRequestAccept,
        messages::ConnectionRequestAcceptAck,
        messages::ConnectionRequestRefuse,
        messages::Data>;
}
// clang-format on