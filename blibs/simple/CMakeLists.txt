cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

project(simple)

file(GLOB_RECURSE simple_SOURCES "*.cpp")

add_library(simple SHARED ${simple_SOURCES})
