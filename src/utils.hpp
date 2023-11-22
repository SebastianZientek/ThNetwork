#pragma once

#include <Arduino.h>

#include "MacAddr.hpp"

namespace utils
{
std::string readingsToJsonString(
    float temp, float hum, MacAddr mac, const std::string &sensorName, unsigned long epochTime);
std::string epochToFormattedDate(unsigned long epochTime);
}  // namespace utils
