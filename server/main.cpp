#include <iostream>

#include "ErrorCode.hpp"  //not used now
#include "ServerApp.hpp"

void runApp(types::IP ip, types::Port port) {
  app::Server server{};
  server.run(ip, port);
}

int main(int argc, char** argv) {
  // TODO server app
  // TODO Error handling

  if (argc != 3) {
    std::cerr << "Please provide IP and port address! e.g. 127.0.0.1 5000"
              << std::endl;
    return -1;
  }

  runApp(types::IP{argv[1]}, types::Port{argv[2]});

  return 0;
}