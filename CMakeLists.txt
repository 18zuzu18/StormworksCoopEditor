cmake_minimum_required(VERSION 3.16)
project(StromworksCoopEditor)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)
include_directories(lib/glfw/include)
include_directories(lib/glad/include)
include_directories(lib/glm)

add_executable(StromworksCoopEditor main.cpp lib/glad/src/glad.c mesh/MeshDecoder.h graphics/Shader.h)

target_link_libraries(StromworksCoopEditor glfw)