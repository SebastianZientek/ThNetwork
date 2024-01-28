#include <CppUTest/TestHarness.h>

#include "FileMock.hpp"
#include "FileSystemStub.hpp"
#include "ConfStorage.hpp"

// clang-format off
TEST_GROUP(ConfStorageTest)  // NOLINT
{   
    void teardown() override{
        mock().checkExpectations();
        mock().clear();
    }
};
// clang-format on

TEST(ConfStorageTest, XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx)  // NOLINT
{
}
