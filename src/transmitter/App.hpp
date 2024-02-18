#pragma once

#include "EspNow.hpp"
#include "Sensor.hpp"
#include "config.hpp"
#include "adapters/Arduino8266Adp.hpp"
#include "transmitter/adapters/IArduino8266Adp.hpp"

class App
{
public:
    void setup();
    void loop();

private:
    constexpr static unsigned long m_usInMin
        = 1e6;  // Should be 6e7, now it's seconds (for debug purposes)
    config::TransmitterConfig m_currentConfiguration;
    
    std::shared_ptr<IArduino8266Adp> m_arduinoAdp{std::make_shared<Arduino8266Adp>()};
    EspNow m_espNow{m_arduinoAdp};
    Sensor m_sensor;
};
