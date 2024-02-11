#pragma once

#include <cinttypes>

struct ArduinoAdp
{
    static void pinMode(uint8_t pin, uint8_t mode);
    static uint8_t digitalRead(uint8_t pin);
    static void digitalWrite(uint8_t pin, uint8_t val);
    static uint8_t getLedBuiltin();

    static const uint8_t LVL_HI;
    static const uint8_t LVL_LO;

    static const uint8_t MODE_OUT;
    static const uint8_t MODE_IN;
};
