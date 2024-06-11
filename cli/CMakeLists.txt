set(MODULE_NAME cli)
set(MODULE_LIB ${MODULE_NAME}_lib)

set(HEADERS
    include/CLI.hpp)
set(SOURCES
    src/CLI.cpp)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::cli ALIAS ${MODULE_LIB})

target_link_libraries(${MODULE_LIB}
PUBLIC
    ${Boost_LIBRARIES}
    lib::core
    lib::network
    lib::types
    lib::utils
    lib::messages
    spdlog::spdlog)
target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

add_executable(${MODULE_NAME} main.cpp)

target_link_libraries(${MODULE_NAME}
PRIVATE
    lib::cli)