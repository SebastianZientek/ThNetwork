#pragma once

#include <ESP8266WiFi.h>
#include <Esp.h>

#include "IEsp8266Adp.hpp"

class Esp8266Adp : public IEsp8266Adp
{
public:
    void feedWatchdog() override
    {
        ESP.wdtFeed();
    }

    void yield() override
    {
        yield();
    }

    bool isResetReasonDeepSleepAwake() override
    {
        auto restartReason = ESP.getResetInfoPtr()->reason;
        return restartReason == REASON_DEEP_SLEEP_AWAKE;
    }
};
