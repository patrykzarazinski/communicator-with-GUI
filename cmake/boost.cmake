include_guard()

find_package(Boost 1.79.0)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost could not be found")
endif()