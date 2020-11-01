cmake_minimum_required(VERSION 3.17)
project(core)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(test-daemon tests/main.cpp)
#set(BUILD_SHARED_LIBS YES)

add_library(daemon-client SHARED client/client.cpp)
add_library(core STATIC core/core.cpp)
