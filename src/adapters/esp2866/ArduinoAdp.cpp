#include "ArduinoAdp.hpp"

#include <Arduino.h>

const uint8_t ArduinoAdp::LVL_HI = HIGH;
const uint8_t ArduinoAdp::LVL_LO = LOW;

const uint8_t ArduinoAdp::MODE_OUT = OUTPUT;
const uint8_t ArduinoAdp::MODE_IN = INPUT;

void ArduinoAdp::pinMode(uint8_t pin, uint8_t mode)
{
    return ::pinMode(pin, mode);
}

uint8_t ArduinoAdp::digitalRead(uint8_t pin)
{
    return ::digitalRead(pin);
}

void ArduinoAdp::digitalWrite(uint8_t pin, uint8_t val)
{
    ::digitalWrite(pin, val);
}

uint8_t ArduinoAdp::getLedBuiltin()
{
    return LED_BUILTIN;
}

unsigned long ArduinoAdp::millis()
{
    return ::millis();
}

void ArduinoAdp::delay(unsigned long milliseconds)
{
    return ::delay(milliseconds);
}
