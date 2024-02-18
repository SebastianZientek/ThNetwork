#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "EspNowPairingManager.hpp"
#include "mocks/Arduino32AdpMock.hpp"
#include "mocks/ConfStorageMock.hpp"

// clang-format off
TEST_GROUP(EspNowPairingManagerTest)  // NOLINT
{   
    void teardown() override{
        mock().checkExpectations();
        mock().clear();
    }
};
// clang-format on

TEST(EspNowPairingManagerTest, EnableAndDisablePairing)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock().ignoreOtherCalls();

    espNowPairingManager.enablePairingForPeriod();
    CHECK_TRUE(espNowPairingManager.isPairingEnabled());

    espNowPairingManager.disablePairing();
    CHECK_FALSE(espNowPairingManager.isPairingEnabled());
}

TEST(EspNowPairingManagerTest, PairingEnabledForPeriod)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(0);
    espNowPairingManager.enablePairingForPeriod(pairingTimeoutMilis);
    CHECK_TRUE(espNowPairingManager.isPairingEnabled());

    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(5);
    espNowPairingManager.update();
    CHECK_TRUE(espNowPairingManager.isPairingEnabled());

    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(15);
    espNowPairingManager.update();
    CHECK_FALSE(espNowPairingManager.isPairingEnabled());
}

TEST(EspNowPairingManagerTest, AddSensorToStorageWhenItPossibe)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock("ConfStorageMock")
        .expectOneCall("isSensorMapped")
        .withParameter("identifier", 123)
        .andReturnValue(false);
    mock("ConfStorageMock").expectOneCall("isAvailableSpaceForNextSensor").andReturnValue(true);
    mock("ConfStorageMock")
        .expectOneCall("addSensor")
        .withParameter("identifier", 123)
        .ignoreOtherParameters();
    mock().ignoreOtherCalls();

    espNowPairingManager.addNewSensorToStorage(123);
}

TEST(EspNowPairingManagerTest, NotAddSensorToStorageWhenSensorIsAlreadyMapped)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock("ConfStorageMock")
        .expectOneCall("isSensorMapped")
        .withParameter("identifier", 123)
        .andReturnValue(true);
    mock("ConfStorageMock").expectNoCall("addSensor");
    mock().ignoreOtherCalls();

    espNowPairingManager.addNewSensorToStorage(123);
}

TEST(EspNowPairingManagerTest, NotAddSensorToStorageWhenThereIsNoMoreSpaceForSensors)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock("ConfStorageMock")
        .expectOneCall("isSensorMapped")
        .withParameter("identifier", 123)
        .andReturnValue(false);
    mock("ConfStorageMock").expectOneCall("isAvailableSpaceForNextSensor").andReturnValue(false);
    mock("ConfStorageMock").expectNoCall("addSensor");
    mock().ignoreOtherCalls();

    espNowPairingManager.addNewSensorToStorage(123);
}

TEST(EspNowPairingManagerTest, UnaddNewSensorToStorage)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock("ConfStorageMock").expectOneCall("removeSensor").withParameter("identifier", 123);
    mock().ignoreOtherCalls();

    espNowPairingManager.unpairSensor(123);
}
