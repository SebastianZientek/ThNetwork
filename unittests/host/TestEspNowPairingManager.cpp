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

TEST(EspNowPairingManagerTest, EnableAndDisablePairing)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    EspNowPairingManager espNowPairingManager(confStorageMock, nullptr);
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
    EspNowPairingManager espNowPairingManager(confStorageMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock().expectOneCall("ArduinoAdp::millis").andReturnValue(0);
    espNowPairingManager.enablePairingForPeriod(pairingTimeoutMilis);
    CHECK_TRUE(espNowPairingManager.isPairingEnabled());

    mock().expectOneCall("ArduinoAdp::millis").andReturnValue(5);
    espNowPairingManager.update();
    CHECK_TRUE(espNowPairingManager.isPairingEnabled());

    mock().expectOneCall("ArduinoAdp::millis").andReturnValue(15);
    espNowPairingManager.update();
    CHECK_FALSE(espNowPairingManager.isPairingEnabled());
}

TEST(EspNowPairingManagerTest, PairSensorWhenItPossibe)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    EspNowPairingManager espNowPairingManager(confStorageMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock().expectOneCall("ConfStorageMock::isSensorMapped").withParameter("identifier", 123).andReturnValue(false);
    mock().expectOneCall("ConfStorageMock::isAvailableSpaceForNextSensor").andReturnValue(true);
    mock().expectOneCall("ConfStorageMock::addSensor").withParameter("identifier", 123).ignoreOtherParameters();
    mock().ignoreOtherCalls();

    espNowPairingManager.pairSensor(123);
}

TEST(EspNowPairingManagerTest, NotPairSensorWhenSensorIsAlreadyMapped)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    EspNowPairingManager espNowPairingManager(confStorageMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock().expectOneCall("ConfStorageMock::isSensorMapped").withParameter("identifier", 123).andReturnValue(true);
    mock().expectNoCall("ConfStorageMock::addSensor");
    mock().ignoreOtherCalls();

    espNowPairingManager.pairSensor(123);
}

TEST(EspNowPairingManagerTest, NotPairSensorWhenThereIsNoMoreSpaceForSensors)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    EspNowPairingManager espNowPairingManager(confStorageMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock().expectOneCall("ConfStorageMock::isSensorMapped").withParameter("identifier", 123).andReturnValue(false);
    mock().expectOneCall("ConfStorageMock::isAvailableSpaceForNextSensor").andReturnValue(false);
    mock().expectNoCall("ConfStorageMock::addSensor");
    mock().ignoreOtherCalls();

    espNowPairingManager.pairSensor(123);
}

TEST(EspNowPairingManagerTest, UnpairSensor)  // NOLINT
{
    auto confStorageMock = std::make_shared<ConfStorageMock>();
    EspNowPairingManager espNowPairingManager(confStorageMock, nullptr);
    constexpr auto pairingTimeoutMilis = 10;

    mock().expectOneCall("ConfStorageMock::removeSensor").withParameter("identifier", 123);
    mock().ignoreOtherCalls();

    espNowPairingManager.unpairSensor(123);
}
