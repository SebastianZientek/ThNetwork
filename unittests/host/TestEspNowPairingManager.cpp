#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "Arduino32AdpMock.hpp"
#include "ConfStorageMock.hpp"
#include "EspNowPairingManager.hpp"

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

    mock().expectOneCall("Arduino32Adp::millis").andReturnValue(0);
    espNowPairingManager.enablePairingForPeriod(pairingTimeoutMilis);
    CHECK_TRUE(espNowPairingManager.isPairingEnabled());

    mock().expectOneCall("Arduino32Adp::millis").andReturnValue(5);
    espNowPairingManager.update();
    CHECK_TRUE(espNowPairingManager.isPairingEnabled());

    mock().expectOneCall("Arduino32Adp::millis").andReturnValue(15);
    espNowPairingManager.update();
    CHECK_FALSE(espNowPairingManager.isPairingEnabled());
}

TEST(EspNowPairingManagerTest, AddSensorToStorageWhenItPossibe)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock()
        .expectOneCall("ConfStorageMock::isSensorMapped")
        .withParameter("identifier", 123)
        .andReturnValue(false);
    mock().expectOneCall("ConfStorageMock::isAvailableSpaceForNextSensor").andReturnValue(true);
    mock()
        .expectOneCall("ConfStorageMock::addSensor")
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

    mock()
        .expectOneCall("ConfStorageMock::isSensorMapped")
        .withParameter("identifier", 123)
        .andReturnValue(true);
    mock().expectNoCall("ConfStorageMock::addSensor");
    mock().ignoreOtherCalls();

    espNowPairingManager.addNewSensorToStorage(123);
}

TEST(EspNowPairingManagerTest, NotAddSensorToStorageWhenThereIsNoMoreSpaceForSensors)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock()
        .expectOneCall("ConfStorageMock::isSensorMapped")
        .withParameter("identifier", 123)
        .andReturnValue(false);
    mock().expectOneCall("ConfStorageMock::isAvailableSpaceForNextSensor").andReturnValue(false);
    mock().expectNoCall("ConfStorageMock::addSensor");
    mock().ignoreOtherCalls();

    espNowPairingManager.addNewSensorToStorage(123);
}

TEST(EspNowPairingManagerTest, UnaddNewSensorToStorage)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    auto arduino32AdpMock = std::make_shared<Arduino32AdpMock>();

    EspNowPairingManager espNowPairingManager(confStorageMock, arduino32AdpMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock().expectOneCall("ConfStorageMock::removeSensor").withParameter("identifier", 123);
    mock().ignoreOtherCalls();

    espNowPairingManager.unpairSensor(123);
}
