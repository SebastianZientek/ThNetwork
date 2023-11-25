#include <CppUTest/TestHarness.h>

#include <array>

#include "MacAddr.hpp"

TEST_GROUP(TestMacAddr)  // NOLINT
{};

TEST(TestMacAddr, ShouldBeAbleToReturnString)  // NOLINT
{
    MacAddr macAddr = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};        // NOLINT
    CHECK_EQUAL(macAddr.str(), std::string("11:22:33:44:55:66"));  // NOLINT
}
