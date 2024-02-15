#include "Arduino32Adp.hpp"

#include <Arduino.h>

void Arduino32Adp::pinMode(uint8_t pin, Mode mode)
{
    switch (mode)
    {
    case Mode::PIN_OUTPUT:
        ::pinMode(pin, OUTPUT);
        break;
    case Mode::PIN_PULLUP:
        ::pinMode(pin, PULLUP);
        break;
    case Mode::PIN_INPUT_PULLUP:
        ::pinMode(pin, INPUT_PULLUP);
        break;
    case Mode::PIN_PULLDOWN:
        ::pinMode(pin, PULLDOWN);
        break;
    case Mode::PIN_INPUT_PULLDOWN:
        ::pinMode(pin, INPUT_PULLDOWN);
        break;
    case Mode::PIN_OPEN_DRAIN:
        ::pinMode(pin, OPEN_DRAIN);
        break;
    case Mode::PIN_OUTPUT_OPEN_DRAIN:
        ::pinMode(pin, OUTPUT_OPEN_DRAIN);
        break;
    case Mode::PIN_ANALOG:
        ::pinMode(pin, ANALOG);
        break;
    }
}

bool Arduino32Adp::digitalRead(uint8_t pin)
{
    return ::digitalRead(pin) == HIGH;
}

void Arduino32Adp::digitalWrite(uint8_t pin, bool val)
{
    ::digitalWrite(pin, val ? HIGH : LOW);
}

uint8_t Arduino32Adp::getLedBuiltin()
{
    return 0;
}

unsigned long Arduino32Adp::millis()
{
    return ::millis();
}

void Arduino32Adp::delay(unsigned long milliseconds)
{
    return delay(milliseconds);
}
