#include "App.hpp"

#include <ArduinoJson.h>
#include <SD.h>

#include "ArduinoJson/Document/StaticJsonDocument.hpp"
#include "ArduinoJson/Json/JsonSerializer.hpp"
#include "boardsettings.hpp"
#include "logger.hpp"

App::App()
{
}

void App::run()
{
    if (systemInit() != Status::OK)
    {
        constexpr auto msInSecond = 1000;
        logger::logErrF("System will be rebooted in %ds",
                        boardsettings::failRebootDelay / msInSecond);
        delay(boardsettings::failRebootDelay);
        ESP.restart();
    }

    auto &sensorsMap = m_config.getSensorsMap();
    m_espNow.init(
        [this, sensorsMap](float temp, float hum, String mac, unsigned long epochTime)
        {
            std::array<char, 5> buf;
            std::sprintf(&buf[0], "%.1f", temp);
            String temperature = buf.data();
            std::sprintf(&buf[0], "%.1f", hum);
            String humidity = buf.data();

            StaticJsonDocument<120> readings{};
            readings["epochTime"] = epochTime;
            readings["temperature"] = temperature;
            readings["humidity"] = humidity;
            readings["id"] = mac;
            auto containsMac = sensorsMap.find(mac) != sensorsMap.end();
            readings["name"] = containsMac ? sensorsMap.at(mac) : mac;
            String jsonString{};
            serializeJson(readings, jsonString);
            m_web.sendEvent(jsonString.c_str(), "new_readings", millis());
        });

    m_web.load("/index.html");
    m_web.startServer();
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

    // m_timeClient.setTimeOffset(3600);
    m_timeClient.update();

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
    String data = file.asString();
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
        file.saveString(m_config.getExampleConfig());
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
