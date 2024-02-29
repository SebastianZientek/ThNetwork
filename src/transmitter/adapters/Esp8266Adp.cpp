#include "Esp8266Adp.hpp"

#include <ESP8266WiFi.h>
#include <Esp.h>

void Esp8266Adp::feedWatchdog() const 
{
    ESP.wdtFeed();
}

void Esp8266Adp::yield() const 
{
    yield();
}

[[nodiscard]] bool Esp8266Adp::isResetReasonDeepSleepAwake() const 
{
    auto restartReason = ESP.getResetInfoPtr()->reason;
    return restartReason == REASON_DEEP_SLEEP_AWAKE;
}
