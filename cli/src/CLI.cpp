#include "CLI.hpp"

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "core/Sender.hpp"
#include "network/Socket.hpp"
#include "spdlog/spdlog.h"

namespace {
void signalHandler(int sig) { spdlog::info("Better not to use ctrl+c"); }

void showHelp() {
  std::cout << "Available commands:" << std::endl;
  std::cout << " info       Information about connected clients" << std::endl;
  std::cout << " close <id> Disconnect client wiht id" << std::endl;
  std::cout << " help       Show this help message" << std::endl;
  std::cout << " exit       Exit the program" << std::endl;
}

}  // namespace

namespace app {
CLI::CLI()
    : socket{std::make_unique<network::Socket>()},
      sender{std::make_unique<core::Sender>()} {}

void CLI::run(types::IP ip, types::Port port) {
  std::signal(SIGINT, signalHandler);

  socket->createSocket(ip, port);

  std::cout << "Welcome to the CLI. Type 'help' for a list of commands."
            << std::endl;

  loop();
}

void CLI::loop() {
  std::string command;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, command);

    if (command.empty()) {
      continue;
    }

    handleCommand(command);
  }
}

void CLI::handleCommand(const std::string& command) {
  std::istringstream iss(command);
  std::string cmd;

  iss >> cmd;

  if (cmd == "help") {
    showHelp();
    return;
  }

  if (cmd == "exit") {
    std::exit(EXIT_SUCCESS);
  }

  if (cmd == "info") {
    // todo print data from database
  } else if (cmd == "close") {
    int id;
    iss >> id;
    if (iss) {
      // todo request to close client
    } else {
      std::cerr << "Invalid input" << std::endl;
    }
  } else {
    std::cerr << "Unknown command: " << cmd << std::endl;
  }
}

CLI::~CLI() { spdlog::info("CLI shutdown"); };
}  // namespace app