#pragma once

#include <cinttypes>

struct ArduinoAdp
{
    static uint8_t digitalRead(uint8_t pin);
    static void digitalWrite(uint8_t pin, uint8_t val);
    static uint8_t getLedBuiltin();

    static const uint8_t LVL_HI;
    static const uint8_t LVL_LO;
};
