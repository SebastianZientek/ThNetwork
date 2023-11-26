#pragma once

#include "EspNow.hpp"
#include "Sensor.hpp"
#include "config.hpp"

class App
{
public:
    void setup();
    void loop();

private:
    constexpr static unsigned long m_usInMin
        = 1e6;  // Should be 6e7, now it's seconds (for debug purposes)
    config::TransmitterConfig m_currentConfiguration;
    EspNow m_espNow;
    Sensor m_sensor;
};
