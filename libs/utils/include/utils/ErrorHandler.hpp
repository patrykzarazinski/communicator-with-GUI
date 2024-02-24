#pragma once

#include <cstdio>   //std::perror
#include <cstdlib>  //std::exit, std::atoi
#include <string>

namespace utils {
class ErrorHandler {
 public:
  static void handleError(const std::string& message) {
    std::perror(message.c_str());
    std::exit(EXIT_FAILURE);
  }
};
}  // namespace utils