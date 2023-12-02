#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "ArduinoAdp.hpp"
#include "MacAddr.hpp"
#include "utils.hpp"

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
    mock().expectOneCall("ArduinoAdp::getLedBuiltin").andReturnValue(LED_BUILTIN);
    mock()
        .expectNCalls(1, "ArduinoAdp::digitalWrite")
        .withParameter("pin", LED_BUILTIN)
        .withParameter("val", ArduinoAdp::LVL_HI);

    utils::switchOffLed();
}

TEST(TestUtils, shouldSwitchOnLed)  // NOLINT
{
    mock().expectOneCall("ArduinoAdp::getLedBuiltin").andReturnValue(LED_BUILTIN);
    mock()
        .expectNCalls(1, "ArduinoAdp::digitalWrite")
        .withParameter("pin", LED_BUILTIN)
        .withParameter("val", ArduinoAdp::LVL_LO);

    utils::switchOnLed();
}
