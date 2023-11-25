#pragma once

#include <Arduino.h>

namespace boardsettings
{
constexpr bool enableLogger = true;
constexpr unsigned long loggerBaud = 9600;
#ifdef UNIT_TESTS
constexpr auto pairButton = 1;
#else
constexpr auto pairButton = D5;
#endif
}  // namespace boardsettings
