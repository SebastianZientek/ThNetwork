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

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

function(buildTests suiteName srcs incls)
    add_executable(${suiteName} ${srcs} ${tests})
    target_include_directories(${suiteName} PRIVATE ${incls})
    target_compile_options(${suiteName} PUBLIC -fsanitize=address -O0 -g)
    target_link_libraries(${suiteName} PRIVATE CppUTest::CppUTest CppUTest::CppUTestExt nlohmann_json::nlohmann_json)
    target_compile_features(${suiteName} PRIVATE cxx_std_17)
    target_compile_definitions(${suiteName} PRIVATE UNIT_TESTS)
    target_link_options(${suiteName} PRIVATE -fsanitize=address)
endfunction()

set(COMMON_INCLS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
)

set(COMMON_TEST_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/MacAddr.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/test_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/common/TestMacAddr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/common/TestSerializer.cpp
)

set(HOST_INCLS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host
)

set(HOST_TEST_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/ConfStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/ReadingsStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/EspNowPairingManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/WebPageMain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/LedIndicator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/Timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/Button.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/WifiConfiguratorWebServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host/WifiConfigurator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/MacAddr.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/test_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestRingBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestRaiiFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestConfStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestReadingsStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestEspNowPairingManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestWebPageMain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestButton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestWifiConfiguratorWebServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/host/tests/TestWifiConfigurator.cpp
)

set(TRANSMITTER_INCLS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transmitter
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/transmitter
)

set(TRANSMITTER_TEST_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transmitter/EspNow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transmitter/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/MacAddr.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/test_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/transmitter/tests/TestEspNow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unittests/transmitter/tests/TestUtils.cpp
)

buildTests(CommonUTs "${COMMON_TEST_SRCS}" "${COMMON_INCLS}")
buildTests(HostUTs "${HOST_TEST_SRCS}" "${HOST_INCLS}")
buildTests(TransmitterUTs "${TRANSMITTER_TEST_SRCS}" "${TRANSMITTER_INCLS}")
