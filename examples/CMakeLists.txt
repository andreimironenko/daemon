# adding the Google_Tests_run target

include(FetchContent)

include_directories(../include)

set(CMAKE_CXX_STANDARD 17)


# linking Google_Tests_run with DateConverter_lib which will be tested
add_executable(daemon-test daemon-test.cpp)
target_link_libraries(daemon-test daemon rt)

