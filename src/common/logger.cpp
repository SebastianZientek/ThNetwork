#include "logger.hpp"

#include <Arduino.h>

namespace logger
{
void init()
{
#ifdef ENABLE_LOGGER
    constexpr auto serialBaudRate = 115200;
    Serial.begin(serialBaudRate);
#endif
}
}  // namespace logger
