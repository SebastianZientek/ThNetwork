#pragma once

#pragma once

#include <CppUTestExt/MockSupport.h>

#include "adapters/IEsp8266Adp.hpp"

class Esp8266AdpMock : public IEsp8266Adp
{
public:
    void feedWatchdog()
    {
        mock("Esp8266Adp").actualCall("feedWatchdog");
    }

    void yield()
    {
        mock("Esp8266Adp").actualCall("yield");
    }
    
    bool isResetReasonDeepSleepAwake()
    {
        return mock("Esp8266Adp")
            .actualCall("isResetReasonDeepSleepAwake")
            .returnBoolValueOrDefault(false);
    }
};
