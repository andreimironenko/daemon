cmake_minimum_required(VERSION 3.16)
project(core)
set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS YES)

include_directories(include)

add_library(daemon-client SHARED client/client.cpp include/client.h)
add_library(daemon STATIC core/core.cpp include/daemon.h)
add_library(mq STATIC mq/mq.cpp include/mq.h mq/mq_.h mq/mq_.cpp)

add_subdirectory(tests)


