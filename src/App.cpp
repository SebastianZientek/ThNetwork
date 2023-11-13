#include "App.hpp"

#include <ArduinoJson.h>
#include <SD.h>

#include "ArduinoJson/Document/StaticJsonDocument.hpp"
#include "ArduinoJson/Json/JsonSerializer.hpp"
#include "MacAddr.hpp"
#include "RaiiFile.hpp"
#include "boardsettings.hpp"
#include "logger.hpp"
#include "utils.hpp"


void App::init()
{
    if (systemInit() != Status::OK)
    {
        constexpr auto msInSecond = 1000;
        logger::logErrF("System will be rebooted in %ds",
                        boardsettings::failRebootDelay / msInSecond);
        delay(boardsettings::failRebootDelay);
        ESP.restart();
    }

    m_espNow.init(
        [this](float temp, float hum, MacAddr mac, unsigned long epochTime)
        {
            auto sensorName = m_config.getSensorName(mac.str()).value_or(mac);
            m_readings.addReading(mac, sensorName, temp, hum, epochTime);
        },
        m_config.getSensorUpdatePeriodMins());

    m_web.startServer(
        [this]
        {
            auto &currentReadings = m_readings.getReadings();
            for (const auto &[macAddr, reading] : currentReadings)
            {
                sendEvent(reading.temperature, reading.humidity, macAddr, reading.epochTime);
            }
        });
}

void App::update()
{
    constexpr auto msToUpdate = 5000;
    auto initTimer = [] { return millis() + msToUpdate; };
    static auto callTimePoint = initTimer();

    if (callTimePoint < millis())
    {
        callTimePoint = initTimer();

        auto &currentReadings = m_readings.getReadings();
        for (const auto &[macAddr, reading] : currentReadings)
        {
            sendEvent(reading.temperature, reading.humidity, macAddr, reading.epochTime);
        }
    }
}

App::Status App::systemInit()
{
    delay(boardsettings::initDelay);
    logger::init();
    if (auto status = initSD(); status != Status::OK)
    {
        return status;
    }
    if (auto status = saveExampleConfig(); status != Status::OK)
    {
        return status;
    }
    if (auto status = readConfig(); status != Status::OK)
    {
        return status;
    }
    if (auto status = connectWiFi(); status != Status::OK)
    {
        return status;
    }

    m_timeClient->update();

    return Status::OK;
}

App::Status App::initSD()
{
    if (!SD.begin())
    {
        logger::logErr("Card Mount Failed");
        return Status::FAIL;
    }

    if (SD.cardType() == CARD_NONE)
    {
        logger::logErr("No SD card attached");
        return Status::FAIL;
    }
    return Status::OK;
}

App::Status App::readConfig()
{
    RaiiFile file("/config.json");
    String data = file->readString();
    if (!m_config.load(data))
    {
        logger::logErr("Reading config error");
        return Status::FAIL;
    }
    return Status::OK;
}

App::Status App::saveExampleConfig()
{
    if (SD.exists("/config_example.json"))
    {
        logger::logInf("config_example.json exists");
    }
    else
    {
        logger::logInf("Saving config_example.json");
        RaiiFile file("/config_example.json", FILE_WRITE);
        file->print(m_config.getExampleConfig());
    }

    return Status::OK;
}

App::Status App::connectWiFi()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(m_config.getWifiSsid(), m_config.getWifiPass());
    logger::logInf("Connecting to WiFi");

    uint8_t wifiConnectionTries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        wifiConnectionTries++;
        delay(boardsettings::wifiConnectDelay);
        logger::logInf(".");

        if (wifiConnectionTries >= boardsettings::wifiConnectionRetriesBeforeReboot)
        {
            logger::logErr("WiFi connection issue, reboot.");
            delay(boardsettings::wifiConnectionIssueRebootDelay);
            ESP.restart();
        }
    }

    logger::logInfF("Connected to %s IP: %s MAC: %s, channel %d", WiFi.SSID(),
                    WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str(), WiFi.channel());

    return Status::OK;
}

void App::sendEvent(float temp, float hum, MacAddr mac, unsigned long epochTime)
{
    auto sensorName = m_config.getSensorName(mac.str()).value_or(mac);
    String jsonString = utils::readingsToJsonString(temp, hum, mac, sensorName, epochTime);
    m_web.sendEvent(jsonString.c_str(), "new_readings", millis());
}
