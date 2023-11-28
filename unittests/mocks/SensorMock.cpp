#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "Sensor.hpp"

void Sensor::init()
{
    mock().actualCall("Sensor::init");
}

std::pair<float, float> Sensor::getTempHum()
{
    mock().actualCall("Sensor::getTempHum");
    return {0, 0};
}
