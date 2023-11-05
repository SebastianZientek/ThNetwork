#include <CppUTest/TestHarness.h>

#include "ReadingsStorage.hpp"

TEST_GROUP(ReadingsStorageTest)  // NOLINT
{};

TEST(ReadingsStorageTest, SaveReadingToFile)  // NOLINT
{
    ReadingsStorage rd;
    const uint8_t addr[8] = {};
    rd.addReading(addr, 100, 200, 1234);

    
}
