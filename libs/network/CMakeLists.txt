set(MODULE_LIB network)

set(HEADERS
    include/network/BaseSocket.hpp
    include/network/ListenSocket.hpp
    include/network/Socket.hpp)
 set(SOURCES
    src/network/BaseSocket.cpp
    src/network/ListenSocket.cpp
    src/network/Socket.cpp)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::network ALIAS ${MODULE_LIB})

target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

target_link_libraries(${MODULE_LIB}
PRIVATE
    lib::types
    lib::utils)