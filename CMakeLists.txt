cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include(cotire)

project(th3d)

file(GLOB_RECURSE th3d_SOURCES "src/*.cpp")
file(GLOB_RECURSE th3d_HEADERS "include/*.hpp")
file(GLOB_RECURSE th3d_SHADERS "shaders/*.glsl")

file(COPY resources DESTINATION .)

include_directories(include)
add_executable(th3d ${th3d_SOURCES})
target_link_libraries(th3d GLEW glfw GL png dl)
cotire(th3d)
