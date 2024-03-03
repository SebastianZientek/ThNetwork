#include "App.hpp"

#include "BoardSettings.hpp"
#include "common/MacAddr.hpp"
#include "common/logger.hpp"
#include "config.hpp"
#include "utils.hpp"

void App::setup()
{
    logger::init();
    logger::logInf("Mac address: %s", m_wifiAdp->macAddress().c_str());

    pinMode(boardSettings::pairButton, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    utils::switchOffLed(m_arduinoAdp);

    if (m_espAdp->isResetReasonDeepSleepAwake())
    {
        m_currentConfiguration = config::readFromRTC();
    }
    else
    {
        m_currentConfiguration = config::readFromEEPROM();
        config::saveToRTC(m_currentConfiguration);
    }

    m_espNow.init(m_currentConfiguration.channel);
    m_sensor.init(boardSettings::sensorSDA, boardSettings::sensorSCL);

    if (digitalRead(boardSettings::pairButton) == LOW)
    {
        logger::logInf("Pairing");
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
    auto [temp, hum] = m_sensor.getTempHum();
    logger::logInf("Temp: %f, hum: %f", temp, hum);
    m_espNow.sendDataToHost(m_currentConfiguration.ID, m_currentConfiguration.targetMac, temp, hum);

    ESP.deepSleep(m_currentConfiguration.sensorUpdatePeriodMins * m_usInMin);
}
