cmake_minimum_required(VERSION 3.21.3)

project(communicator-with-GUI CXX)

set(CMAKE_CXX_STANDARD 20)
message(STATUS "CMAKE_CXX_STANDARD: " ${CMAKE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_NO_CYCLES ON)
get_Property(propValue GLOBAL PROPERTY GLOBAL_DEPENDS_NO_CYCLES)
message(STATUS "GLOBAL_DEPENDS_NO_CYCLES: " ${propValue})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(boost)

add_custom_target(format COMMAND ${CMAKE_CURRENT_LIST_DIR}/scripts/formatter.sh)

add_subdirectory(libs)
add_subdirectory(client)
add_subdirectory(server)
