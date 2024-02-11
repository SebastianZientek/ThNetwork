#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "EspNowPairingManager.hpp"
#include "ConfStorageMock.hpp"

// clang-format off
TEST_GROUP(EspNowPairingManagerTest)  // NOLINT
{   
    void teardown() override{
        mock().checkExpectations();
        mock().clear();
    }
};
// clang-format on

TEST(EspNowPairingManagerTest, XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    EspNowPairingManager espNowPairingManager(confStorageMock, nullptr);
    // mock().expectOneCall("File::close");
    // fs::File fileMock;
    // RaiiFile someFile(fileMock);
    CHECK_TRUE(false);
}
