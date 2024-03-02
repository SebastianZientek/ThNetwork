#include "Arduino32Adp.hpp"

#include <Arduino.h>

void Arduino32Adp::pinMode(uint8_t pin, Mode mode) const
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

IArduino32Adp::Lvl Arduino32Adp::digitalRead(uint8_t pin) const
{
    return ::digitalRead(pin) == HIGH ? Lvl::High : Lvl::Low;
}

void Arduino32Adp::digitalWrite(uint8_t pin, Lvl lvl) const
{
    ::digitalWrite(pin, lvl == Lvl::High ? HIGH : LOW);
}

uint8_t Arduino32Adp::getLedBuiltin() const
{
    return 0;
}

unsigned long Arduino32Adp::millis() const
{
    return ::millis();
}

void Arduino32Adp::delay(unsigned long milliseconds) const
{
    return ::delay(milliseconds);
}
