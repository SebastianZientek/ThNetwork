#include "Timer.hpp"

Timer::Timer(const std::shared_ptr<IArduino32Adp> &arduinoAdp)
    : m_arduinoAdp(arduinoAdp)
{
}

void Timer::setCallback(const FunType &fun)
{
    m_function = fun;
}

void Timer::start(std::size_t timeoutMillis, bool repeat)
{
    m_period = timeoutMillis;
    m_startTime = m_arduinoAdp->millis();

    m_stopped = false;
    m_repeat = repeat;
}

void Timer::stop()
{
    m_stopped = true;
}

void Timer::update()
{
    if (!m_stopped && m_period + m_startTime < m_arduinoAdp->millis())
    {
        m_function();

        if (m_repeat)
        {
            m_startTime = m_arduinoAdp->millis();
        }
        else
        {
            m_stopped = true;
        }
    }
}
