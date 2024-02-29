#pragma once

#include <cstddef>
#include <functional>
#include <memory>

#include "adapters/IArduino32Adp.hpp"

class Timer
{
    using FunType = std::function<void()>;

public:
    Timer(const std::shared_ptr<IArduino32Adp> &arduinoAdp);

    void setCallback(const FunType &fun);
    void start(std::size_t timeoutMillis, bool repeat = false);
    void stop();
    void update();

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
