#include "EspAdp.hpp"

#include <ESP8266WiFi.h>
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
    delay(timeout);
}

bool EspAdp::isResetReasonDeepSleepAwake()
{
    auto restartReason = ESP.getResetInfoPtr()->reason;
    return restartReason == REASON_DEEP_SLEEP_AWAKE;
}
