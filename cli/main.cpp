#include <iostream>

#include "CLI.hpp"
#include "types/IP.hpp"
#include "types/Port.hpp"

void runApp(types::IP ip, types::Port port) {
  app::CLI cli{};
  cli.run(ip, port);
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Please provide IP and port address! e.g. 127.0.0.1 5000"
              << std::endl;
    return -1;
  }

  runApp(types::IP{argv[1]}, types::Port{argv[2]});

  return 0;
}