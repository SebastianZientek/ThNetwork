#pragma once

#include <Arduino.h>

namespace utils
{
std::string epochToFormattedDate(unsigned long epochTime);
std::string getSHA256(std::string data);
}  // namespace utils
