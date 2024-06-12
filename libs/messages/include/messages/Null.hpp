#pragma once

/*
Purpose of this message is to discard unnecessary data on the socket.
During closing connections wiht cli or client it send tcp fin,ack etc., which
cause problem during deserialization
*/

namespace messages {
struct Null {};
}  // namespace messages