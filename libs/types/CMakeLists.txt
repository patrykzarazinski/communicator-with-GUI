set(MODULE_LIB types)

set(HEADERS
    include/types/ClientID.hpp
    include/types/FD.hpp
    include/types/IP.hpp
    include/types/Port.hpp)
# set(SOURCES
#     src/types/Adress.cpp)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::types ALIAS ${MODULE_LIB})

target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

#due to lack .cpp files linker are not able to determine linker language
set_target_properties(${MODULE_LIB} PROPERTIES LINKER_LANGUAGE CXX)