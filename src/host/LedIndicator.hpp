#pragma once

#include <cstddef>

#include "Timer.hpp"
#include "common/logger.hpp"
#include "adapters/ArduinoAdp.hpp"

class LedIndicator
{
public:
    LedIndicator(std::size_t pin)
        : m_ledPin(pin)
    {
        ArduinoAdp::pinMode(m_ledPin, ArduinoAdp::MODE_OUT);
        m_blinkTimer.setCallback(
            [this, ledOn = false]() mutable
            {
                ledOn = !ledOn;
                ArduinoAdp::digitalWrite(m_ledPin, ledOn ? ArduinoAdp::LVL_HI : ArduinoAdp::LVL_LO);
            });
    }

    void switchOn(bool state)
    {
        m_blinkTimer.stop();
        ArduinoAdp::digitalWrite(m_ledPin, state ? ArduinoAdp::LVL_HI : ArduinoAdp::LVL_LO);
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
