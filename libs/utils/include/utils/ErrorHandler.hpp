#pragma once

#include <cstdlib>  //std::exit, std::atoi
#include <string>

#include "spdlog/spdlog.h"

namespace utils {
class ErrorHandler {
 public:
  static void handleError(const std::string& message) {
    spdlog::error("{}", message);
    std::exit(EXIT_FAILURE);
  }
};
}  // namespace utils