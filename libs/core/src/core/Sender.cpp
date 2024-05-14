#include "core/Sender.hpp"

#include <sys/socket.h>
#include <unistd.h>  // close, read

#include <iostream>
#include <variant>

#include "main.pb.h"
#include "types/Buffer.hpp"
#include "utils/ErrorHandler.hpp"
#include "utils/Overload.hpp"

namespace {
protobuf::main::Msg serializeData(messages::Data& in) {
  protobuf::main::Msg out;

  out.set_msg_type(protobuf::main::MessageType::DATA);
  out.mutable_payload()->mutable_data()->mutable_data()->set_data(in.data);

  return out;
}

protobuf::main::Msg serializeConnectionRequest(
    messages::ConnectionRequest& in) {
  protobuf::main::Msg out;

  out.set_msg_type(protobuf::main::MessageType::CONNECTION_REQUEST);
  out.mutable_payload()
      ->mutable_connection_request()
      ->mutable_transaction_id()
      ->set_transaction_id(in.transactionId);

  return out;
}

protobuf::main::Msg serializeConnectionRequestAccept(
    messages::ConnectionRequestAccept& in) {
  protobuf::main::Msg out;

  out.set_msg_type(protobuf::main::MessageType::CONNECTION_REQUEST_ACCEPT);
  out.mutable_payload()
      ->mutable_connection_request_accept()
      ->mutable_transaction_id()
      ->set_transaction_id(in.transactionId);
  out.mutable_payload()
      ->mutable_connection_request_accept()
      ->mutable_uuid()
      ->set_uuid(in.uuid);

  return out;
}

protobuf::main::Msg serializeConnectionRequestAcceptAck(
    messages::ConnectionRequestAcceptAck& in) {
  protobuf::main::Msg out;

  out.set_msg_type(protobuf::main::MessageType::CONNECTION_REQUEST_ACCEPT_ACK);
  out.mutable_payload()
      ->mutable_connection_request_accept_ack()
      ->mutable_transaction_id()
      ->set_transaction_id(in.transactionId);

  out.mutable_payload()
      ->mutable_connection_request_accept_ack()
      ->mutable_uuid()
      ->set_uuid(in.transactionId);

  return out;
}

protobuf::main::Msg serializeConnectionRequestRefuse(
    messages::ConnectionRequestRefuse& in) {
  protobuf::main::Msg out;

  out.set_msg_type(protobuf::main::MessageType::CONNECTION_REQUEST_REFUSE);
  out.mutable_payload()
      ->mutable_connection_request_refuse()
      ->mutable_transaction_id()
      ->set_transaction_id(in.transactionId);
  out.mutable_payload()->mutable_connection_request_refuse()->set_cause(
      static_cast<::protobuf::types::Cause>(in.cause));

  return out;
}

protobuf::main::Msg serializeConnectionDisconnection(
    messages::ConnectionDisconnection& in) {
  protobuf::main::Msg out;

  out.set_msg_type(protobuf::main::MessageType::CONNECTION_DISCONNECTION);
  out.mutable_payload()
      ->mutable_connection_disconnection()
      ->mutable_uuid()
      ->set_uuid(in.uuid);

  return out;
}
}  // namespace

namespace core {
//  size of buffer and size of protobuf messages is not synchronized
// TODO synchronize with tcp/ip protocol, mtu etc.

void send_internal(const types::FD& socket, protobuf::main::Msg msg) {
  types::Buffer buffer(1024, '\0');

  if (not msg.SerializeToString(&buffer)) {
    utils::ErrorHandler::handleError("SerializeToString return false");
  }

  if (auto i = send(socket, buffer.data(), buffer.size(), 0); i == -1) {
    utils::ErrorHandler::handleError("Error with send");
  }
}

void Sender::send(const types::FD socket, messages::Message message) {
  protobuf::main::Msg out;

  std::visit(utils::overload{
                 [&](messages::Data& msg) {
                   send_internal(socket, serializeData(msg));
                 },
                 [&](messages::ConnectionRequest& msg) {
                   send_internal(socket, serializeConnectionRequest(msg));
                 },
                 [&](messages::ConnectionRequestAccept& msg) {
                   send_internal(socket, serializeConnectionRequestAccept(msg));
                 },
                 [&](messages::ConnectionRequestAcceptAck& msg) {
                   send_internal(socket,
                                 serializeConnectionRequestAcceptAck(msg));
                 },
                 [&](messages::ConnectionRequestRefuse& msg) {
                   send_internal(socket, serializeConnectionRequestRefuse(msg));
                 },
                 [&](messages::ConnectionDisconnection& msg) {
                   send_internal(socket, serializeConnectionDisconnection(msg));
                 }},
             message);
}
}  // namespace core