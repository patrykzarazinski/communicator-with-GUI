set(MODULE_NAME server)
set(MODULE_LIB ${MODUE_NAME}_lib)

set(HEADERS
    include/Epoll.hpp
    include/ServerApp.hpp)
set(SOURCES
    src/Epoll.cpp
    src/ServerApp.cpp)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::server ALIAS ${MODULE_LIB})

target_link_libraries(${MODULE_LIB} 
PUBLIC
    ${Boost_LIBRARIES}
    lib::core
    lib::network
    lib::types
    lib::utils
    lib::messages)
target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

add_executable(${MODULE_NAME} main.cpp)

target_link_libraries(${MODULE_NAME}
PRIVATE
    lib::server)