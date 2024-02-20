#include <CppUTest/TestHarness.h>

#include "mocks/FileMock.hpp"
#include "adapters/RaiiFile.hpp"

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
    mock("File").expectOneCall("close");
    fs::File fileMock;
    RaiiFile someFile(fileMock);
}
