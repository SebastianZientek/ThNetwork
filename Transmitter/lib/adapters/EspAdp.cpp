#include "EspAdp.hpp"

#include <Esp.h>

void EspAdp::feedWatchdog()
{
    ESP.wdtFeed();
}

void EspAdp::yield()
{
    yield();
}

void EspAdp::wait(unsigned long timeout)
{
    auto startTime = millis();
    while (startTime + timeout > millis())
    {
        ESP.wdtFeed();
    }
}
