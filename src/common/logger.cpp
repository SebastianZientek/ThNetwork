#include "logger.hpp"

#include <Arduino.h>

// #include "boardsettings.hpp"

namespace logger
{
void init()
{
#ifdef ENABLE_LOGGER
    Serial.begin(115200);
#endif
}
}  // namespace logger
