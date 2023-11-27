#include "App.hpp"

#include "adapters/EspAdp.hpp"
#include "adapters/WiFiAdp.hpp"
#include "boardsettings.hpp"
#include "common/MacAddr.hpp"
#include "common/logger.hpp"
#include "config.hpp"
#include "utils.hpp"

void App::setup()
{
    logger::init();
    logger::logInf(WiFiAdp::macAddress().c_str());
    pinMode(boardsettings::pairButton, INPUT_PULLUP);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    if (EspAdp::isResetReasonDeepSleepAwake())
    {
        m_currentConfiguration = config::readFromRTC();
    }
    else
    {
        m_currentConfiguration = config::readFromEEPROM();
        config::saveToRTC(m_currentConfiguration);
    }

    m_espNow.init(m_currentConfiguration.channel);
    m_sensor.init();

    if (digitalRead(boardsettings::pairButton) == LOW)
    {
        if (auto transmitterConfigOpt = m_espNow.pair(); transmitterConfigOpt)
        {
            m_currentConfiguration = transmitterConfigOpt.value();

            config::saveToEEPROM(m_currentConfiguration);
            config::saveToRTC(m_currentConfiguration);
        }
        else
        {
            logger::logWrn("Can't pair!");
        }
    }

    logger::logInf("Initialized, sensor update period: %d",
                   m_currentConfiguration.sensorUpdatePeriodMins);
}

void App::loop()
{
    logger::logInf("Current config mac: %s, ch: %d", m_currentConfiguration.targetMac.str(),
                   m_currentConfiguration.channel);

    auto [temp, hum] = m_sensor.getTempHum();
    logger::logInf("Temp: %f, hum: %f", temp, hum);
    m_espNow.sendDataToHost(m_currentConfiguration.targetMac, temp, hum);

    ESP.deepSleep(m_currentConfiguration.sensorUpdatePeriodMins * m_usInMin);
}
