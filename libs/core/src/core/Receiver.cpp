#include "core/Receiver.hpp"

#include <sys/socket.h>
#include <unistd.h>  // close, read

#include <cstring>
#include <iostream>

#include "main.pb.h"
#include "types/Buffer.hpp"
#include "utils/ErrorHandler.hpp"

namespace {
bool checkReadBytes(ssize_t& readBytes, const types::FD& socket) {
  if (readBytes > 0) {
    return true;
  }

  if (readBytes == -1) {
    close(socket);
    utils::ErrorHandler::handleError("recv() failed");
  } else if (readBytes == 0) {
    // TODO add logger lib
    std::cout << "Connection closed by peer" << std::endl;
    close(socket);
  }

  return false;
}

// TODO complete desrialization functions
messages::Data deserializeData(const protobuf::main::Payload& in) {
  messages::Data out;

  if (in.has_data() and in.data().has_data()) {
    out.data = in.data().data().data();
  } else {
    utils::ErrorHandler::handleError("Problem with deserializeData");
  }

  return out;
}

messages::ConnectionRequest deserializeConnectionRequest(
    const protobuf::main::Payload& in) {
  messages::ConnectionRequest out;

  if (in.has_connection_request() and
      in.connection_request().has_transaction_id()) {
    out.transactionId =
        in.connection_request().transaction_id().transaction_id();
  } else {
    utils::ErrorHandler::handleError(
        "Problem with deserializeConnectionRequest");
  }

  return out;
}

messages::ConnectionRequestAccept deserializeConnectionRequestAccept(
    const protobuf::main::Payload& in) {
  messages::ConnectionRequestAccept out;

  if (in.has_connection_request_accept() and
      in.connection_request_accept().has_transaction_id()) {
    out.transactionId =
        in.connection_request_accept().transaction_id().transaction_id();
  } else {
    utils::ErrorHandler::handleError(
        "Problem with transactionId at deserializeConnectionRequestAccept");
  }

  if (in.has_connection_request_accept() and
      in.connection_request_accept().has_uuid()) {
    out.uuid = in.connection_request_accept().uuid().uuid();
  } else {
    utils::ErrorHandler::handleError(
        "Problem with uuid at deserializeConnectionRequestAccept");
  }

  return out;
}

messages::ConnectionRequestAcceptAck deserializeConnectionRequestAcceptAck(
    const protobuf::main::Payload& in) {
  messages::ConnectionRequestAcceptAck out;

  if (in.has_connection_request_accept_ack() and
      in.connection_request_accept_ack().has_transaction_id()) {
    out.transactionId =
        in.connection_request_accept_ack().transaction_id().transaction_id();
  } else {
    utils::ErrorHandler::handleError(
        "Problem with transactionId at deserializeConnectionRequestAcceptAck");
  }

  if (in.has_connection_request_accept_ack() and
      in.connection_request_accept_ack().has_uuid()) {
    out.uuid = in.connection_request_accept_ack().uuid().uuid();
  } else {
    utils::ErrorHandler::handleError(
        "Problem with uuid at deserializeConnectionRequestAcceptAck");
  }

  return out;
}

messages::ConnectionRequestRefuse deserializeConnectionRequestRefuse(
    const protobuf::main::Payload& in) {
  messages::ConnectionRequestRefuse out;

  if (in.has_connection_request_refuse() and
      in.connection_request_refuse().has_transaction_id()) {
    out.transactionId =
        in.connection_request_refuse().transaction_id().transaction_id();
  } else {
    utils::ErrorHandler::handleError(
        "Problem with transactionId at deserializeConnectionRequestRefuse");
  }

  if (in.has_connection_request_refuse()) {
    out.cause =
        static_cast<types::Cause>(in.connection_request_refuse().cause());
  } else {
    utils::ErrorHandler::handleError(
        "Problem with cause at deserializeConnectionRequestRefuse");
  }

  return out;
}

messages::ConnectionDisconnection deserializeConnectionDisconnection(
    const protobuf::main::Payload& in) {
  messages::ConnectionDisconnection out;

  if (in.has_connection_disconnection() and
      in.connection_disconnection().has_uuid()) {
    out.uuid = in.connection_disconnection().uuid().uuid();
  } else {
    utils::ErrorHandler::handleError(
        "Problem with deserializeConnectionDisconnection");
  }

  return out;
}
}  // namespace

namespace core {
types::Buffer receive_internal(const types::FD& socket) {
  // TODO buffer is fixed for 1024. Is it enough?
  types::Buffer buffer(1024, '\0');

  ssize_t readBytes =
      recv(socket, buffer.data(), buffer.size(), 0);  // MSG_DONTWAIT

  types::Buffer protobufBuffer(readBytes, '\0');
  std::memcpy(protobufBuffer.data(), buffer.data(), readBytes);

  return checkReadBytes(readBytes, socket) ? protobufBuffer : types::Buffer{};
}

messages::Message Receiver::receive(const types::FD socket) {
  types::Buffer buffer = receive_internal(socket);

  protobuf::main::Msg msg;
  if (not msg.ParseFromString(buffer)) {
    // TODO enhance error handlers, the log "ParseFromString returned false:
    // Success" at error is printed
    utils::ErrorHandler::handleError("ParseFromString returned false");
  }
  switch (msg.msg_type()) {
    case protobuf::main::MessageType::DATA:
      return deserializeData(msg.payload());
    case protobuf::main::MessageType::CONNECTION_REQUEST:
      return deserializeConnectionRequest(msg.payload());
    case protobuf::main::MessageType::CONNECTION_REQUEST_ACCEPT:
      return deserializeConnectionRequestAccept(msg.payload());
    case protobuf::main::MessageType::CONNECTION_REQUEST_ACCEPT_ACK:
      return deserializeConnectionRequestAcceptAck(msg.payload());
    case protobuf::main::MessageType::CONNECTION_REQUEST_REFUSE:
      return deserializeConnectionRequestRefuse(msg.payload());
    case protobuf::main::MessageType::CONNECTION_DISCONNECTION:
      return deserializeConnectionDisconnection(msg.payload());
    default:
      utils::ErrorHandler::handleError("Not recognized msg_type occured");
  }

  return messages::Message();  // For compilator satisfaction
}
}  // namespace core