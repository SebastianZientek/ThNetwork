#include "Arduino8266Adp.hpp"

#include <Arduino.h>
#include <Wire.h>

#include "pins_arduino.h"

void Arduino8266Adp::pinMode(uint8_t pin, Mode mode)
{
    switch (mode)
    {
    case Mode::PIN_INPUT:
        ::pinMode(pin, INPUT);
        break;
    case Mode::PIN_INPUT_PULLUP:
        ::pinMode(pin, INPUT_PULLUP);
        break;
    case Mode::PIN_INPUT_PULLDOWN_16:
        ::pinMode(pin, INPUT_PULLDOWN_16);
        break;
    case Mode::PIN_OUTPUT:
        ::pinMode(pin, OUTPUT);
        break;
    case Mode::PIN_OUTPUT_OPEN_DRAIN:
        ::pinMode(pin, OUTPUT_OPEN_DRAIN);
        break;
    case Mode::PIN_WAKEUP_PULLUP:
        ::pinMode(pin, WAKEUP_PULLUP);
        break;
    case Mode::PIN_WAKEUP_PULLDOWN:
        ::pinMode(pin, WAKEUP_PULLDOWN);
        break;
    case Mode::PIN_SPECIAL:
        ::pinMode(pin, SPECIAL);
        break;
    }
}

bool Arduino8266Adp::digitalRead(uint8_t pin)
{
    return ::digitalRead(pin) == HIGH;
}

void Arduino8266Adp::digitalWrite(uint8_t pin, bool val)
{
    ::digitalWrite(pin, val ? HIGH : LOW);
}

uint8_t Arduino8266Adp::getLedBuiltin()
{
    return LED_BUILTIN;
}

unsigned long Arduino8266Adp::millis()
{
    return ::millis();
}

void Arduino8266Adp::delay(unsigned long milliseconds)
{
    return ::delay(milliseconds);
}

void Arduino8266Adp::setupWire(int sda, int scl)
{
    Wire.begin(sda, scl);
}
