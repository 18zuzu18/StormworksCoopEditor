cmake_minimum_required(VERSION 3.16)
project(StromworksCoopEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)
include_directories(lib/glfw/include)
include_directories(lib/glad/include)
include_directories(lib/glm)
include_directories(lib/rapidxml)

add_executable(StromworksCoopEditor main.cpp lib/glad/src/glad.c mesh/MeshDecoder.h graphics/Shader.h graphics/Mesh.h logic/ComponentManager.h logic/Component.h graphics/SurfaceRenderer.h)

target_link_libraries(StromworksCoopEditor glfw)