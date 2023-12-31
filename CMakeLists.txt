cmake_minimum_required(VERSION 3.27)
project(UnitTests)

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

function(buildTests suiteName srcs incls mocks tests)
    add_executable(${suiteName} ${srcs} ${mocks} ${tests})
    target_include_directories(${suiteName} PRIVATE ${incls})
    target_compile_options(${suiteName} PUBLIC -fsanitize=address -O0 -g)
    target_link_libraries(${suiteName} PRIVATE CppUTest::CppUTest CppUTest::CppUTestExt)
    target_compile_features(${suiteName} PRIVATE cxx_std_17)
    target_compile_definitions(${suiteName} PRIVATE UNIT_TESTS)
    target_link_options(${suiteName} PRIVATE -fsanitize=address)
endfunction()

set(SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transmitter/EspNow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transmitter/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/MacAddr.cpp
)

set(INCLS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/adapters
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transmitter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/mocks
)

set(MOCKS
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/mocks/ArduinoAdpStub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/mocks/EspAdpStub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/mocks/EspNowAdpMock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/mocks/WifiAdpMock.cpp
)

set(TR_TESTS
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/test_main.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/transmitter/TestEspNow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/transmitter/TestUtils.cpp
)

set(HS_TESTS
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/test_main.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/TestRingBuffer.cpp
)

set(CM_TESTS
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/test_main.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/common/TestMacAddr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/common/TestSerializer.cpp
)


buildTests(TransmitterUTs "${SRCS}" "${INCLS}" "${MOCKS}" "${TR_TESTS}")
buildTests(HostUTs "${SRCS}" "${INCLS}" "${MOCKS}" "${HS_TESTS}")
buildTests(CommonUTs "${SRCS}" "${INCLS}" "${MOCKS}" "${CM_TESTS}")

# add_executable(UnitTests ${SRCS} ${MOCKS} ${TESTS})
# target_include_directories(UnitTests PRIVATE ${INCLS})
# target_compile_options(UnitTests PUBLIC -fsanitize=address -O0 -g)
# target_link_libraries(UnitTests PRIVATE CppUTest::CppUTest CppUTest::CppUTestExt)
# target_compile_features(UnitTests PRIVATE cxx_std_17)
# target_compile_definitions(UnitTests PRIVATE UNIT_TESTS)
# target_link_options(UnitTests PRIVATE -fsanitize=address)


# # unit tests with coverage
# add_executable(UnitTestsGcov ${SRCS} ${MOCKS} ${TESTS})
# target_include_directories(UnitTestsGcov PRIVATE ${INCLS})
# target_compile_options(UnitTestsGcov PUBLIC -fprofile-arcs -ftest-coverage -O0 -g)
# target_link_libraries(UnitTestsGcov PRIVATE gcov CppUTest::CppUTest CppUTest::CppUTestExt)
# target_compile_features(UnitTestsGcov PRIVATE cxx_std_17)
# target_compile_definitions(UnitTestsGcov PRIVATE UNIT_TESTS)

# add_custom_target(TestsGcovTarget
#     COMMAND find . -name "*.gcda" -delete
#     COMMAND lcov -i --exclude '/usr/*' --exclude '*unittests*' --exclude '*build*' -d ${CMAKE_BINARY_DIR} --capture --output-file coverage_initial.info --ignore-errors mismatch
#     COMMAND $<TARGET_FILE:UnitTestsGcov>
#     COMMAND lcov --exclude '/usr/*' --exclude '*unittests*' --exclude '*build*' -d ${CMAKE_BINARY_DIR} --capture --output-file coverage_test.info --ignore-errors mismatch
#     COMMAND lcov -a coverage_initial.info -a coverage_test.info -o coverage.info
#     COMMAND genhtml --function-coverage --branch-coverage coverage.info --output-directory coverage
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )