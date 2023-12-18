set(MODULE_NAME server)
set(MODULE_LIB ${MODUE_NAME}_lib)

set(HEADERS
    include/ServerApp.hpp
    include/ServerContext.hpp
    include/Socket.hpp
    include/SocketCreator.hpp
    include/TCPSocket.hpp
    include/TCPSocketCreator.hpp)
set(SOURCES
    src/ServerApp.cpp
    src/TCPSocket.cpp
    src/TCPSocketCreator.cpp)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::server ALIAS ${MODULE_LIB})

target_link_libraries(${MODULE_LIB} PUBLIC ${Boost_LIBRARIES} lib::types)
target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

add_executable(${MODULE_NAME} main.cpp)

target_link_libraries(${MODULE_NAME}
PRIVATE
    lib::server)