cmake_minimum_required(VERSION 3.27)
project(HomeMonitorHost)

include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        master # or use release tag, eg. v4.0
    )
set(TESTS OFF CACHE BOOL "Switch off CppUTest Test build")
# Needed because of confict with libc (see more in CppUTest docs)
set(CPPUTEST_MEM_LEAK_DETECTION_DISABLED ON CACHE BOOL "Switch off CppUTest leak detection" FORCE)
FetchContent_MakeAvailable(CppUTest)

add_executable(UnitTests
    unittests/test_main.cpp
    unittests/TestExample.cpp
)

target_include_directories(UnitTests PRIVATE
    src
    lib/adapters
    unittests/mocks
)
target_compile_features(UnitTests PRIVATE cxx_std_17)
target_compile_definitions(UnitTests PRIVATE UNIT_TESTS)
target_compile_options(UnitTests PUBLIC -fsanitize=address -O0 -g)
target_link_options(UnitTests PRIVATE -fsanitize=address)

target_link_libraries(UnitTests PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)
