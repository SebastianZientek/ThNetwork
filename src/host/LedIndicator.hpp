#pragma once

#include <cstddef>
#include <memory>

#include "Timer.hpp"
#include "adapters/IArduino32Adp.hpp"

class LedIndicator
{
public:
    LedIndicator(const std::shared_ptr<IArduino32Adp> &arduinoAdp, std::size_t pin);
    void switchOn(bool state);
    void blinking();
    void update();

private:
    constexpr static auto m_periodMilis = 200;

    std::size_t m_ledPin;
    std::shared_ptr<IArduino32Adp> m_arduinoAdp;
    Timer m_blinkTimer;
};
