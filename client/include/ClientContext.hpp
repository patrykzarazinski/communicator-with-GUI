#pragma once

#include <boost/asio.hpp>

#include "types/IP.hpp"
#include "types/Port.hpp"

namespace client {
struct ClientContext {
  ClientContext(types::IP& ip, types::Port& port)
      : address{boost::asio::ip::make_address_v4(ip)},
        endpoint{address, static_cast<uint_least16_t>(std::stoi(port))},
        socket{io_context} {}

  boost::asio::io_context io_context;
  boost::asio::ip::address address;
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::socket socket;
};
}  // namespace client