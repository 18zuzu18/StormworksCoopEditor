cmake_minimum_required(VERSION 3.16)
project(StromworksCoopEditor)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)
include_directories(lib/glad/include)

add_executable(StromworksCoopEditor main.cpp lib/glad/src/glad.c)

target_link_libraries(StromworksCoopEditor glfw)