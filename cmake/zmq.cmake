include_guard()

set(DEPENDENCY_NAME cppzmq)

find_package(${DEPENDENCY_NAME} QUIET)

if(NOT ${${DEPENDENCY_NAME}_FOUND})
    message(FATAL_ERROR "${DEPENDENCY_NAME} could not be found")
endif()