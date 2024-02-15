#pragma once

#pragma once

#include <CppUTestExt/MockSupport.h>

#include <cinttypes>

#include "adapters/esp32/IArduino32Adp.hpp"

class Arduino32AdpMock : public IArduino32Adp
{
public:
    void pinMode(uint8_t pin, IArduino32Adp::Mode mode)
    {
        mock()
            .actualCall("Arduino32Adp::digitalWrite")
            .withParameter("pin", pin)
            .withParameter("mode", static_cast<int>(mode));
    }

    bool digitalRead(uint8_t pin)
    {
        return mock()
            .actualCall("Arduino32Adp::digitalRead")
            .withParameter("pin", pin)
            .returnBoolValueOrDefault(false);
    }

    void digitalWrite(uint8_t pin, bool val)
    {
        mock()
            .actualCall("Arduino32Adp::digitalWrite")
            .withParameter("pin", pin)
            .withParameter("val", val);
    }

    uint8_t getLedBuiltin()
    {
        return mock().actualCall("Arduino32Adp::getLedBuiltin").returnIntValueOrDefault(0);
    }

    unsigned long millis()
    {
        return mock().actualCall("Arduino32Adp::millis").returnIntValueOrDefault(0);
    }

    void delay(unsigned long milliseconds)
    {
        mock().actualCall("Arduino32Adp::delay").withParameter("milliseconds", milliseconds);
    }
};
