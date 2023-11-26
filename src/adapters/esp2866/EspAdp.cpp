#include "EspAdp.hpp"

#include <Esp.h>
#include <ESP8266WiFi.h>

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

bool EspAdp::isResetReasonDeepSleepAwake()
{
    auto restartReason = ESP.getResetInfoPtr()->reason;
    return restartReason == REASON_DEEP_SLEEP_AWAKE;
}