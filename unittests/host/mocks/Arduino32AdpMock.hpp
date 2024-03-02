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

    [[nodiscard]] IArduino32Adp::Lvl digitalRead(uint8_t pin) const override
    {
        static auto defaultState = IArduino32Adp::Lvl::Low;
        auto *returnVal = mock("Arduino32Adp")
            .actualCall("digitalRead")
            .withParameter("pin", pin)
            .returnPointerValueOrDefault(&defaultState);

        return *static_cast<IArduino32Adp::Lvl *>(returnVal);
    }

    void digitalWrite(uint8_t pin, IArduino32Adp::Lvl val) const override
    {
        mock("Arduino32Adp")
            .actualCall("digitalWrite")
            .withParameter("pin", pin)
            .withParameter("val", static_cast<int>(val));
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
