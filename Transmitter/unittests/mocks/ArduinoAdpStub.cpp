#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "ArduinoAdp.hpp"

const uint8_t ArduinoAdp::HI = 1;
const uint8_t ArduinoAdp::LO = 0;

uint8_t ArduinoAdp::digitalRead([[maybe_unused]] uint8_t pin)
{
    return 0;
}

void ArduinoAdp::digitalWrite([[maybe_unused]] uint8_t pin, [[maybe_unused]] uint8_t val)
{
}

uint8_t ArduinoAdp::getLedBuiltin()
{
    return 0;
}
