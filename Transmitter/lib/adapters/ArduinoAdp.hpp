#pragma once

#include <cinttypes>

struct ArduinoAdp {
    static uint8_t digitalRead(uint8_t pin);
    static void digitalWrite(uint8_t pin, uint8_t val);
    static uint8_t getLedBuiltin();

    constexpr static uint8_t HIGH = 1;
    constexpr static uint8_t LOW = 0;
};