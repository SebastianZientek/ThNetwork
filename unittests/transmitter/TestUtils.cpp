#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "ArduinoAdp.hpp"
#include "MacAddr.hpp"
#include "utils.hpp"

TEST_GROUP(TestUtils)  // NOLINT
{
    void teardown() override
    {
        mock().clear();
        mock().checkExpectations();
    }

    constexpr static auto LED_BUILTIN = 9;
};

TEST(TestUtils, shouldToggleLedPinFromHighToLow)  // NOLINT
{
    mock().expectOneCall("ArduinoAdp::getLedBuiltin").andReturnValue(LED_BUILTIN);
    mock()
        .expectNCalls(1, "ArduinoAdp::digitalRead")
        .withParameter("pin", LED_BUILTIN)
        .andReturnValue(ArduinoAdp::LVL_HI);
    mock()
        .expectNCalls(1, "ArduinoAdp::digitalWrite")
        .withParameter("pin", LED_BUILTIN)
        .withParameter("val", ArduinoAdp::LVL_LO);

    utils::toggleLed();
}

TEST(TestUtils, shouldToggleLedPinFromLowToHigh)  // NOLINT
{
    mock().expectOneCall("ArduinoAdp::getLedBuiltin").andReturnValue(LED_BUILTIN);
    mock()
        .expectNCalls(1, "ArduinoAdp::digitalRead")
        .withParameter("pin", LED_BUILTIN)
        .andReturnValue(ArduinoAdp::LVL_LO);
    mock()
        .expectNCalls(1, "ArduinoAdp::digitalWrite")
        .withParameter("pin", LED_BUILTIN)
        .withParameter("val", ArduinoAdp::LVL_HI);

    utils::toggleLed();
}
