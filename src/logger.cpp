#include "logger.hpp"

#include <Arduino.h>

#include "boardsettings.hpp"

namespace logger
{
void init()
{
    if constexpr (boardsettings::enableLogger)
    {
        Serial.begin(boardsettings::loggerBaud);
    }
}
}  // namespace logger
