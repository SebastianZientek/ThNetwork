#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "MacAddr.hpp"
#include "utils.hpp"
#include "mocks/Arduino8266AdpMock.hpp"

TEST_GROUP(TestUtils)  // NOLINT
{
    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }

    constexpr static auto LED_BUILTIN = 9;
};

TEST(TestUtils, shouldSwitchOffLed)  // NOLINT
{
    auto arduinoAdpMock = std::make_shared<Arduino8266AdpMock>();

    mock("Arduino8266Adp").expectOneCall("getLedBuiltin").andReturnValue(LED_BUILTIN);
    mock("Arduino8266Adp")
        .expectOneCall("digitalWrite")
        .withParameter("pin", LED_BUILTIN)
        .withParameter("val", true);

    utils::switchOffLed(arduinoAdpMock);
}

TEST(TestUtils, shouldSwitchOnLed)  // NOLINT
{
    auto arduinoAdpMock = std::make_shared<Arduino8266AdpMock>();

    mock("Arduino8266Adp").expectOneCall("getLedBuiltin").andReturnValue(LED_BUILTIN);
    mock("Arduino8266Adp")
        .expectOneCall("digitalWrite")
        .withParameter("pin", LED_BUILTIN)
        .withParameter("val", false);

    utils::switchOnLed(arduinoAdpMock);
}
