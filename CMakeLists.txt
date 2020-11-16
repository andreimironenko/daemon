cmake_minimum_required(VERSION 3.16)

include(FetchContent)

project(core)
set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS YES)

include_directories(include)

add_library(mq STATIC mq/mq.cpp include/mq.h mq/mq_.h mq/mq_.cpp)
add_library(daemon STATIC src/daemon_base.cpp include/daemon.h src/daemon_base_.cpp src/daemon_base_.h)
add_library(daemon-cli STATIC src/daemon_cli.cpp include/daemon_cli.h src/daemon_cli_.cpp src/daemon_cli_.h)

target_link_libraries(daemon mq boost_program_options)
target_link_libraries(daemon-cli mq nlohmann_json)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG master)

FetchContent_GetProperties(json)

if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR})
endif()


add_subdirectory(tests)
add_subdirectory(examples)


