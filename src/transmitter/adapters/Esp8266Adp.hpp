#pragma once

#include <ESP8266WiFi.h>
#include <Esp.h>

#include "IEsp8266Adp.hpp"

class Esp8266Adp : public IEsp8266Adp
{
public:
    void feedWatchdog() const override
    {
        ESP.wdtFeed();
    }

    void yield() const override
    {
        yield();
    }

    [[nodiscard]] bool isResetReasonDeepSleepAwake() const override
    {
        auto restartReason = ESP.getResetInfoPtr()->reason;
        return restartReason == REASON_DEEP_SLEEP_AWAKE;
    }
};
