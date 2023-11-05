cmake_minimum_required(VERSION 3.27)
project(HomeMonitorHost)

set(CPPUTEST_MEM_LEAK_DETECTION_DISABLED ON CACHE BOOL "Disable cppu test leak detection")

include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        master # or use release tag, eg. v4.0
)
set(TESTS OFF CACHE BOOL "Switch off CppUTest Test build")
FetchContent_MakeAvailable(CppUTest)

add_executable(UnitTests
    unittests/TestReadingsStorage.cpp
    unittests/test_main.cpp
    
    src/ReadingsStorage.cpp
    src/MacAddr.cpp)

target_include_directories(UnitTests PRIVATE src unittests/mocks)
target_compile_options(UnitTests PUBLIC -fsanitize=address -Os -g)
target_link_options(UnitTests PRIVATE -fsanitize=address)
target_link_libraries(UnitTests PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)
