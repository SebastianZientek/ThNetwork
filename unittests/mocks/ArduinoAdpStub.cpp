#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "ArduinoAdp.hpp"

const uint8_t ArduinoAdp::LVL_HI = 1;
const uint8_t ArduinoAdp::LVL_LO = 0;

uint8_t ArduinoAdp::digitalRead([[maybe_unused]] uint8_t pin)
{
    return mock().actualCall("ArduinoAdp::digitalRead").withParameter("pin", pin).returnIntValue();
}

void ArduinoAdp::digitalWrite([[maybe_unused]] uint8_t pin, [[maybe_unused]] uint8_t val)
{
    mock()
        .actualCall("ArduinoAdp::digitalWrite")
        .withParameter("pin", pin)
        .withParameter("val", val);
}

uint8_t ArduinoAdp::getLedBuiltin()
{
    return mock().actualCall("ArduinoAdp::getLedBuiltin").returnIntValue();
}
