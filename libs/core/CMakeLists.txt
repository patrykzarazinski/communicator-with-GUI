set(MODULE_LIB core)

set(HEADERS
    include/core/Receiver.hpp
    include/core/Sender.hpp)
set(SOURCES
    src/core/Receiver.cpp
    src/core/Sender.cpp)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::core ALIAS ${MODULE_LIB})

target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

target_link_libraries(${MODULE_LIB}
PRIVATE
    lib::types
    lib::utils
    lib::messages
    proto-objects)
   # spdlog::spdlog)