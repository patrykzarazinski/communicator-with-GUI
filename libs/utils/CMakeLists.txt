set(MODULE_LIB utils)

set(HEADERS
    include/utils/ErrorHandler.hpp)
set(SOURCES)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::utils ALIAS ${MODULE_LIB})

target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

#due to lack .cpp files linker are not able to determine linker language
set_target_properties(${MODULE_LIB} PROPERTIES LINKER_LANGUAGE CXX)