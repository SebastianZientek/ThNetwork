cmake_minimum_required(VERSION 3.27)
project(HomeMonitorHost)

include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        master # or use release tag, eg. v4.0
)
set(TESTS OFF CACHE BOOL "Switch off CppUTest Test build")
FetchContent_MakeAvailable(CppUTest)

add_executable(UnitTests
    unittests/test_main.cpp
    unittests/TestRingBuffer.cpp)

target_include_directories(UnitTests PRIVATE src)
target_link_libraries(UnitTests PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)
