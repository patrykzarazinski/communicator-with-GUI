include_guard()

find_package(Protobuf 4.25.3 CONFIG REQUIRED)

if(NOT Protobuf_FOUND)
    message(FATAL_ERROR "Protobuf could not be found")
else()
    message(STATUS "Protobuf library have been found")
endif()

file(GLOB PROTO_FILES ${CMAKE_SOURCE_DIR}/proto/*.proto)

add_library(proto-objects OBJECT ${PROTO_FILES})
target_link_libraries(proto-objects PUBLIC protobuf::libprotobuf)

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(REMOVE_RECURSE ${PROTO_BINARY_DIR})
file(MAKE_DIRECTORY ${PROTO_BINARY_DIR})

target_include_directories(proto-objects PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

protobuf_generate(
    TARGET proto-objects
    IMPORT_DIRS "${CMAKE_SOURCE_DIR}/proto"
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")