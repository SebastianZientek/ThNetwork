#pragma once

#include <Arduino.h>

#include <cstddef>

#include "Timer.hpp"
#include "common/logger.hpp"

class LedIndicator
{
public:
    LedIndicator(std::size_t pin)
        : m_ledPin(pin)
    {
        pinMode(m_ledPin, OUTPUT);
        m_blinkTimer.setCallback(
            [this, ledOn = false]() mutable
            {
                ledOn = !ledOn;
                digitalWrite(m_ledPin, ledOn ? HIGH : LOW);
            });
    }

    void switchOn(bool state)
    {
        m_blinkTimer.stop();
        digitalWrite(m_ledPin, state ? HIGH : LOW);
    }

    void blinking()
    {
        m_blinkTimer.start(m_periodMilis, true);
    }

    void update()
    {
        m_blinkTimer.update();
    }

private:
    constexpr static auto m_periodMilis = 200;

    std::size_t m_ledPin;
    Timer m_blinkTimer;
};
