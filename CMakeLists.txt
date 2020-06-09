cmake_minimum_required(VERSION 3.16)
project(StromworksCoopEditor)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

add_executable(StromworksCoopEditor main.cpp)

target_link_libraries(StromworksCoopEditor glfw)