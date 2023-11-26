#include "ArduinoAdp.hpp"

#include <Arduino.h>

const uint8_t ArduinoAdp::HI = HIGH;
const uint8_t ArduinoAdp::LO = LOW;

uint8_t ArduinoAdp::digitalRead(uint8_t pin) {
    return ::digitalRead(pin);
}

void ArduinoAdp::digitalWrite(uint8_t pin, uint8_t val) {
    ::digitalWrite(pin, val);
}

uint8_t ArduinoAdp::getLedBuiltin()
{
    return LED_BUILTIN;
}