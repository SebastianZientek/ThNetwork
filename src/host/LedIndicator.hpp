#pragma once

#include <cstddef>
#include <memory>

#include "Timer.hpp"
#include "adapters/esp32/IArduino32Adp.hpp"
#include "common/logger.hpp"

class LedIndicator
{
public:
    LedIndicator(std::shared_ptr<IArduino32Adp> arduinoAdp, std::size_t pin)
        : m_ledPin(pin)
        , m_arduinoAdp(arduinoAdp)
        , m_blinkTimer(arduinoAdp)
    {
        m_arduinoAdp->pinMode(m_ledPin, IArduino32Adp::Mode::PIN_OUTPUT);
        m_blinkTimer.setCallback(
            [this, ledOn = false]() mutable
            {
                ledOn = !ledOn;
                m_arduinoAdp->digitalWrite(m_ledPin, ledOn);
            });
    }

    void switchOn(bool state)
    {
        m_blinkTimer.stop();
        m_arduinoAdp->digitalWrite(m_ledPin, state);
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
    std::shared_ptr<IArduino32Adp> m_arduinoAdp;
    Timer m_blinkTimer;
};
