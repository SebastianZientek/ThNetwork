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

set(SRCS "")
set(MOCKS "")

set(TESTS
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/test_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/TestExample.cpp
)

set(INCLS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/adapters
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/mocks
)

add_executable(UnitTests ${SRCS} ${MOCKS} ${TESTS})
target_include_directories(UnitTests PRIVATE ${INCLS})
target_compile_options(UnitTests PUBLIC -fsanitize=address -O0 -g)
target_link_libraries(UnitTests PRIVATE CppUTest::CppUTest CppUTest::CppUTestExt)
target_compile_features(UnitTests PRIVATE cxx_std_17)
target_compile_definitions(UnitTests PRIVATE UNIT_TESTS)
target_link_options(UnitTests PRIVATE -fsanitize=address)


# unit tests with coverage
add_executable(UnitTestsGcov ${SRCS} ${MOCKS} ${TESTS})
target_include_directories(UnitTestsGcov PRIVATE ${INCLS})
target_compile_options(UnitTestsGcov PUBLIC -fprofile-arcs -ftest-coverage -O0 -g)
target_link_libraries(UnitTestsGcov PRIVATE gcov CppUTest::CppUTest CppUTest::CppUTestExt)
target_compile_features(UnitTestsGcov PRIVATE cxx_std_17)
target_compile_definitions(UnitTestsGcov PRIVATE UNIT_TESTS)

# add_custom_target(TestsGcovTarget
#     COMMAND find . -name "*.gcda" -delete
#     COMMAND lcov -i --exclude '/usr/*'  --exclude '*unittests*' --exclude '*Transmitter/build*' --exclude '*Common/*' -d ${CMAKE_BINARY_DIR} --capture --output-file coverage_initial.info --ignore-errors mismatch
#     COMMAND $<TARGET_FILE:UnitTestsGcov>
#     COMMAND lcov --exclude '/usr/*' --exclude '*unittests*' --exclude '*Transmitter/build*' --exclude '*Common/*' -d ${CMAKE_BINARY_DIR} --capture --output-file coverage_test.info --ignore-errors mismatch
#     COMMAND lcov -a coverage_initial.info -a coverage_test.info -o coverage.info
#     COMMAND genhtml --function-coverage --branch-coverage coverage.info --output-directory coverage
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )