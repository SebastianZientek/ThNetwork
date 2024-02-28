#pragma once

#include <Arduino.h>

namespace boardsettings
{
#ifdef UNIT_TESTS
constexpr auto pairButton = 1;
#else
constexpr auto pairButton = D2;
#endif
}  // namespace boardsettings
