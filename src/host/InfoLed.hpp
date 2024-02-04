#pragma once

#include <Arduino.h>

#include <cstddef>
#include "common/logger.hpp"

class InfoLed
{
public:
    InfoLed(std::size_t pin)
        : m_ledPin(pin)
    {
        pinMode(m_ledPin, OUTPUT);
    }

    void switchOn(bool state)
    {
        digitalWrite(m_ledPin, state ? HIGH : LOW);
        m_state = Mode::NORMAL;
    }

    void blinking()
    {
        m_state = Mode::BLINKING;
    }

    void update()
    {
        if (m_state == Mode::BLINKING)
        {
            if (m_lastChangeTime + m_periodMilis < millis())
            {
                m_lastChangeTime = millis();
                m_ledOn = !m_ledOn;
                digitalWrite(m_ledPin, m_ledOn ? HIGH : LOW);
            }
        }
    }

private:
    enum Mode
    {
        NORMAL,
        BLINKING
    };

    constexpr static auto m_periodMilis = 200;

    std::size_t m_ledPin;
    std::size_t m_lastChangeTime = 0;
    Mode m_state = NORMAL;
    bool m_ledOn = false;
};
