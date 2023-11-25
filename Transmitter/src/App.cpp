#include "App.hpp"

#include <ESP8266WiFi.h>

#include "MacAddr.hpp"
#include "boardsettings.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "utils.hpp"

void App::setup()
{
    logger::init();
    logger::logInf(WiFi.macAddress());
    pinMode(boardsettings::pairButton, INPUT_PULLUP);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    auto restartReason = ESP.getResetInfoPtr()->reason;
    if (restartReason != REASON_DEEP_SLEEP_AWAKE)
    {
        m_currentConfiguration = config::readFromEEPROM();
        config::saveToRTC(m_currentConfiguration);
    }
    else
    {
        m_currentConfiguration = config::readFromRTC();
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
            logger::logErr("Can't pair!");
        }
    }

    logger::logInfF("Initialized, sensor update period: %d",
                    m_currentConfiguration.sensorUpdatePeriodMins);
}

void App::loop()
{
    logger::logInfF("Current config mac: %s, ch: %d",
                    m_currentConfiguration.targetMac.str().c_str(), m_currentConfiguration.channel);

    auto [temp, hum] = m_sensor.getTempHum();
    logger::logInfF("Temp: %f, hum: %f", temp, hum);
    m_espNow.sendDataToHost(m_currentConfiguration.targetMac, temp, hum);

    ESP.deepSleep(m_currentConfiguration.sensorUpdatePeriodMins * m_usInMin);
}
