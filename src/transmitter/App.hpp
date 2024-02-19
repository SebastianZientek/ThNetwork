#pragma once

#include "EspNow.hpp"
#include "Sensor.hpp"
#include "adapters/Arduino8266Adp.hpp"
#include "adapters/Esp8266Adp.hpp"
#include "adapters/EspNow8266Adp.hpp"
#include "adapters/IArduino8266Adp.hpp"
#include "adapters/Wifi8266Adp.hpp"
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

    std::shared_ptr<IArduino8266Adp> m_arduinoAdp{std::make_shared<Arduino8266Adp>()};
    std::shared_ptr<IEsp8266Adp> m_espAdp{std::make_shared<Esp8266Adp>()};
    std::shared_ptr<IWifi8266Adp> m_wifiAdp{std::make_shared<Wifi8266Adp>()};

    EspNow m_espNow{m_arduinoAdp, m_espAdp, std::make_shared<EspNow8266Adp>()};
    Sensor m_sensor;
};
