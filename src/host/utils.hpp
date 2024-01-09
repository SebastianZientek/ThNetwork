#pragma once

#include <Arduino.h>

#include "common/MacAddr.hpp"

namespace utils
{
std::string readingsToJsonString(
    float temp, float hum, MacAddr mac, const std::string &sensorName, unsigned long epochTime);
std::string epochToFormattedDate(unsigned long epochTime);
std::string getSHA256(std::string data);
}  // namespace utils
