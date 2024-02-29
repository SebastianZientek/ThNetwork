#include "LedIndicator.hpp"

LedIndicator::LedIndicator(const std::shared_ptr<IArduino32Adp> &arduinoAdp, std::size_t pin)
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

void LedIndicator::switchOn(bool state)
{
    m_blinkTimer.stop();
    m_arduinoAdp->digitalWrite(m_ledPin, state);
}

void LedIndicator::blinking()
{
    m_blinkTimer.start(m_periodMilis, true);
}

void LedIndicator::update()
{
    m_blinkTimer.update();
}
