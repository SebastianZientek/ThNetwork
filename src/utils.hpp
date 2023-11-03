#pragma once

#include <Arduino.h>

#include "MacAddr.hpp"

namespace utils
{
String readingsToJsonString(
    float temp, float hum, MacAddr mac, const String &sensorName, unsigned long epochTime);
}
