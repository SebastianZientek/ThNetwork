#pragma once

#include <CppUTestExt/MockSupport.h>

#include "adapters/IEsp8266Adp.hpp"

class Esp8266AdpMock : public IEsp8266Adp
{
public:
    void feedWatchdog() const override
    {
        mock("Esp8266Adp").actualCall("feedWatchdog");
    }

    void yield() const override
    {
        mock("Esp8266Adp").actualCall("yield");
    }

    [[nodiscard]] bool isResetReasonDeepSleepAwake() const override
    {
        return mock("Esp8266Adp")
            .actualCall("isResetReasonDeepSleepAwake")
            .returnBoolValueOrDefault(false);
    }
};
