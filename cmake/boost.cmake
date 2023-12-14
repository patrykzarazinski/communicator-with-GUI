include_guard()

find_package(Boost 1.74.0 REQUIRED COMPONENTS thread)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost could not be found")
else()
    message(STATUS "Boost library have been found")
endif()