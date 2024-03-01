#pragma once

#include <CppUTestExt/MockSupport.h>

#include <cinttypes>

#include "adapters/IArduino32Adp.hpp"

class Arduino32AdpMock : public IArduino32Adp
{
public:
    void pinMode(uint8_t pin, Mode mode) const override
    {
        mock("Arduino32Adp")
            .actualCall("pinMode")
            .withParameter("pin", pin)
            .withParameter("mode", static_cast<int>(mode));
    }

    [[nodiscard]] bool digitalRead(uint8_t pin) const override
    {
        return mock("Arduino32Adp")
            .actualCall("digitalRead")
            .withParameter("pin", pin)
            .returnBoolValueOrDefault(false);
    }

    void digitalWrite(uint8_t pin, bool val) const override
    {
        mock("Arduino32Adp")
            .actualCall("digitalWrite")
            .withParameter("pin", pin)
            .withParameter("val", val);
    }

    [[nodiscard]] uint8_t getLedBuiltin() const override
    {
        return mock("Arduino32Adp").actualCall("getLedBuiltin").returnIntValueOrDefault(0);
    }

    [[nodiscard]] unsigned long millis() const override
    {
        return mock("Arduino32Adp").actualCall("millis").returnIntValueOrDefault(0);
    }

    void delay(unsigned long milliseconds) const override
    {
        mock("Arduino32Adp").actualCall("delay").withParameter("milliseconds", milliseconds);
    }
};
