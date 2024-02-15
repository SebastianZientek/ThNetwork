#pragma once

#include <cstddef>
#include <functional>
#include <memory>

#include "adapters/esp32/IArduino32Adp.hpp"

class Timer
{
    using FunType = std::function<void()>;

public:
    Timer(std::shared_ptr<IArduino32Adp> arduinoAdp)
        : m_arduinoAdp(arduinoAdp)
    {
    }

    void setCallback(const FunType &fun)
    {
        m_function = fun;
    }

    void start(std::size_t timeoutMillis, bool repeat = false)
    {
        m_period = timeoutMillis;
        m_startTime = m_arduinoAdp->millis();

        m_stopped = false;
        m_repeat = repeat;
    }

    void stop()
    {
        m_stopped = true;
    }

    void update()
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

private:
    FunType m_function = []
    {
    };
    bool m_stopped = false;
    bool m_repeat = false;
    std::size_t m_period = 0;
    std::size_t m_startTime = 0;

    std::shared_ptr<IArduino32Adp> m_arduinoAdp;
};
