cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall")

project(th3d)

#add_definitions(-DDEBUG)

file(GLOB_RECURSE th3d_SOURCES "src/*.cpp")
file(GLOB_RECURSE th3d_HEADERS "src/*.hpp")

file(COPY resources DESTINATION .)

include_directories(include)
add_executable(th3d ${th3d_SOURCES} ${th3d_HEADERS})
target_link_libraries(th3d GLEW glfw GL png lua5.3 assimp)
