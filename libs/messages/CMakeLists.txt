set(MODULE_LIB messages)

set(HEADERS
    include/messages/ConnectionDisconnection.hpp
    include/messages/ConnectionRequest.hpp
    include/messages/ConnectionRequestAccept.hpp
    include/messages/ConnectionRequestAcceptAck.hpp
    include/messages/ConnectionRequestRefuse.hpp
    include/messages/Data.hpp
    include/messages/Message.hpp)
set(SOURCES)

add_library(${MODULE_LIB} ${HEADERS} ${SOURCES})
add_library(lib::messages ALIAS ${MODULE_LIB})

target_include_directories(${MODULE_LIB} PUBLIC include PRIVATE src)

#due to lack .cpp files linker are not able to determine linker language
set_target_properties(${MODULE_LIB} PROPERTIES LINKER_LANGUAGE CXX)