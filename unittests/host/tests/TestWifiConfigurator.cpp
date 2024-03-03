#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <functional>
#include <memory>
#include <nlohmann/json.hpp>

#include "CppUTest/UtestMacros.h"
#include "WifiConfigurator.hpp"
#include "adapters/IWifi32Adp.hpp"
#include "mocks/Arduino32AdpMock.hpp"
#include "mocks/Wifi32AdpMock.hpp"

TEST_GROUP(WifiConfiguratorTest)  // NOLINT
{
    void setup() override
    {
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }

    std::shared_ptr<Arduino32AdpMock> arduinoAdpMock{std::make_shared<Arduino32AdpMock>()};
    std::shared_ptr<Wifi32AdpMock> wifiAdpMock{std::make_shared<Wifi32AdpMock>()};

    constexpr static auto delayBetweenConnectionRetiresMs = 1000;
    constexpr static auto maxConnectionRetries = 10;
};

TEST(WifiConfiguratorTest, CanConnectToWifi)  // NOLINT
{
    WiFiConfigurator wifiConfigurator{arduinoAdpMock, wifiAdpMock};

    mock("Wifi32AdpMock")
        .expectOneCall("setMode")
        .withParameter("mode", static_cast<int>(IWifi32Adp::Mode::AP_STA));

    mock("Wifi32AdpMock")
        .expectOneCall("init")
        .withStringParameter("ssid", "some ssid")
        .withStringParameter("pass", "some pass");

    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(0);

    wifiConfigurator.connect("some ssid", "some pass");
    CHECK_TRUE(wifiConfigurator.status() == WiFiConfigurator::Status::CONNECTION_ONGOING);

    mock("Arduino32Adp")
        .expectOneCall("millis")
        .andReturnValue(delayBetweenConnectionRetiresMs / 2);
    // mock("Arduino32Adp").expectOneCall("millis").andReturnValue(delayBetweenConnectionRetiresMs /
    // 2);
    wifiConfigurator.update();
    CHECK_TRUE(wifiConfigurator.status() == WiFiConfigurator::Status::CONNECTION_ONGOING);

    static auto disconnectedStatus = IWifi32Adp::Status::DISCONNECTED;
    mock("Arduino32Adp")
        .expectOneCall("millis")
        .andReturnValue(delayBetweenConnectionRetiresMs * 1);
    mock("Arduino32Adp")
        .expectOneCall("millis")
        .andReturnValue(delayBetweenConnectionRetiresMs * 2);
    mock("Wifi32AdpMock").expectOneCall("getStatus").andReturnValue(&disconnectedStatus);
    wifiConfigurator.update();
    CHECK_TRUE(wifiConfigurator.status() == WiFiConfigurator::Status::CONNECTION_ONGOING);

    static auto connectedStatus = IWifi32Adp::Status::CONNECTED;
    mock("Arduino32Adp")
        .expectOneCall("millis")
        .andReturnValue(delayBetweenConnectionRetiresMs * 3);
    mock("Arduino32Adp")
        .expectOneCall("millis")
        .andReturnValue(delayBetweenConnectionRetiresMs * 4);
    mock("Wifi32AdpMock").expectOneCall("getStatus").andReturnValue(&connectedStatus);
    mock("Wifi32AdpMock").expectOneCall("getSsid").ignoreOtherParameters();
    mock("Wifi32AdpMock").expectOneCall("getLocalIp").ignoreOtherParameters();
    mock("Wifi32AdpMock").expectOneCall("getMacAddr").ignoreOtherParameters();
    mock("Wifi32AdpMock").expectOneCall("getChannel").ignoreOtherParameters();
    wifiConfigurator.update();
    CHECK_TRUE(wifiConfigurator.status() == WiFiConfigurator::Status::CONNECTION_SUCCESS);
}

TEST(WifiConfiguratorTest, ReportErrorWhenCantConnectAfterTenRetries)  // NOLINT
{
    WiFiConfigurator wifiConfigurator{arduinoAdpMock, wifiAdpMock};

    mock("Wifi32AdpMock")
        .expectOneCall("setMode")
        .withParameter("mode", static_cast<int>(IWifi32Adp::Mode::AP_STA));

    mock("Wifi32AdpMock")
        .expectOneCall("init")
        .withStringParameter("ssid", "some ssid")
        .withStringParameter("pass", "some pass");

    mock("Arduino32Adp").expectOneCall("millis").andReturnValue(0);
    wifiConfigurator.connect("some ssid", "some pass");
    CHECK_TRUE(wifiConfigurator.status() == WiFiConfigurator::Status::CONNECTION_ONGOING);

    static auto disconnectedStatus = IWifi32Adp::Status::DISCONNECTED;
    for (int retryIdx = 1; retryIdx <= maxConnectionRetries; ++retryIdx)
    {
        mock("Arduino32Adp")
            .expectNCalls(2, "millis")
            .andReturnValue(delayBetweenConnectionRetiresMs * retryIdx);
        mock("Wifi32AdpMock").expectOneCall("getStatus").andReturnValue(&disconnectedStatus);
        wifiConfigurator.update();
    }

    CHECK_TRUE(wifiConfigurator.status() == WiFiConfigurator::Status::CONNECTION_FAILURE);
}
