#pragma once

#pragma once

#include <CppUTestExt/MockSupport.h>

#include <cinttypes>

#include "adapters/IArduino32Adp.hpp"

class Arduino32AdpMock : public IArduino32Adp
{
public:
    void pinMode(uint8_t pin, Mode mode) override
    {
        mock("Arduino32Adp")
            .actualCall("digitalWrite")
            .withParameter("pin", pin)
            .withParameter("mode", static_cast<int>(mode));
    }

    bool digitalRead(uint8_t pin) override
    {
        return mock("Arduino32Adp")
            .actualCall("digitalRead")
            .withParameter("pin", pin)
            .returnBoolValueOrDefault(false);
    }

    void digitalWrite(uint8_t pin, bool val) override
    {
        mock("Arduino32Adp")
            .actualCall("digitalWrite")
            .withParameter("pin", pin)
            .withParameter("val", val);
    }

    uint8_t getLedBuiltin() override
    {
        return mock("Arduino32Adp").actualCall("getLedBuiltin").returnIntValueOrDefault(0);
    }

    unsigned long millis() override
    {
        return mock("Arduino32Adp").actualCall("millis").returnIntValueOrDefault(0);
    }

    void delay(unsigned long milliseconds) override
    {
        mock("Arduino32Adp").actualCall("delay").withParameter("milliseconds", milliseconds);
    }
};
