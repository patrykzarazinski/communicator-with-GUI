set(MODULE_NAME client)
set(MODULE_LIB ${MODULE_NAME}_lib)

set(HEADERS
    include/ClientApp.hpp)
set(SOURCES
    src/ClientApp.cpp)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::client ALIAS ${MODULE_LIB})

target_link_libraries(${MODULE_LIB} PRIVATE lib::types)
target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

add_executable(${MODULE_NAME} main.cpp)

target_link_libraries(${MODULE_NAME}
    lib::client
    cppzmq)
