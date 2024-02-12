#include <CppUTestExt/MockSupport.h>

#include "ArduinoAdp.hpp"

const uint8_t ArduinoAdp::LVL_HI = 1;
const uint8_t ArduinoAdp::LVL_LO = 0;

static const uint8_t MODE_OUT = 1;
static const uint8_t MODE_IN = 0;

void ArduinoAdp::pinMode(uint8_t pin, uint8_t mode)
{
    mock().actualCall("ArduinoAdp::pinMode").withParameter("pin", pin).withParameter("mode", mode);
}

uint8_t ArduinoAdp::digitalRead(uint8_t pin)
{
    return mock()
        .actualCall("ArduinoAdp::digitalRead")
        .withParameter("pin", pin)
        .returnIntValueOrDefault(0);
}

void ArduinoAdp::digitalWrite(uint8_t pin, uint8_t val)
{
    mock()
        .actualCall("ArduinoAdp::digitalWrite")
        .withParameter("pin", pin)
        .withParameter("val", val);
}

uint8_t ArduinoAdp::getLedBuiltin()
{
    return mock().actualCall("ArduinoAdp::getLedBuiltin").returnIntValueOrDefault(0);
}

unsigned long ArduinoAdp::millis()
{
    return mock().actualCall("ArduinoAdp::millis").returnIntValueOrDefault(0);
}
