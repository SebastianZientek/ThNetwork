#pragma once

#include <CppUTestExt/MockSupport.h>

#include <cinttypes>

#include "adapters/IArduino8266Adp.hpp"

class Arduino8266AdpMock : public IArduino8266Adp
{
public:
    void pinMode(uint8_t pin, Mode mode) const override
    {
        mock("Arduino8266Adp")
            .actualCall("digitalWrite")
            .withParameter("pin", pin)
            .withParameter("mode", static_cast<int>(mode));
    }

    [[nodiscard]] bool digitalRead(uint8_t pin) const override
    {
        return mock("Arduino8266Adp")
            .actualCall("digitalRead")
            .withParameter("pin", pin)
            .returnBoolValueOrDefault(false);
    }

    void digitalWrite(uint8_t pin, bool val) const override
    {
        mock("Arduino8266Adp")
            .actualCall("digitalWrite")
            .withParameter("pin", pin)
            .withParameter("val", val);
    }

    [[nodiscard]] uint8_t getLedBuiltin() const override
    {
        return mock("Arduino8266Adp").actualCall("getLedBuiltin").returnIntValueOrDefault(0);
    }

    [[nodiscard]] unsigned long millis() const override
    {
        return mock("Arduino8266Adp").actualCall("millis").returnIntValueOrDefault(0);
    }

    void delay(unsigned long milliseconds) const override
    {
        mock("Arduino8266Adp").actualCall("delay").withParameter("milliseconds", milliseconds);
    }

    void setupWire(int sda, int scl) const override
    {
        mock("Arduino8266Adp")
            .actualCall("setupWire")
            .withParameter("sda", sda)
            .withParameter("scl", scl);
    }
};
