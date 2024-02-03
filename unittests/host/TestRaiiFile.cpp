#include <CppUTest/TestHarness.h>

#include "FileMock.hpp"
#include "FileSystemStub.hpp"
#include "RaiiFile.hpp"

// clang-format off
TEST_GROUP(RaiiFileTest)  // NOLINT
{   
    void teardown() override{
        mock().checkExpectations();
        mock().clear();
    }
};
// clang-format on

TEST(RaiiFileTest, ShouldCloseFileAfterGoingOutOfTheScope)  // NOLINT
{
    mock().expectOneCall("FileMock::close");
    FileMock fileMock;
    RaiiFile<FileMock> someFile(fileMock);
}
