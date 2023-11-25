#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "ArduinoAdp.hpp"

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
