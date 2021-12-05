project(client)

add_subdirectory(include)
add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)