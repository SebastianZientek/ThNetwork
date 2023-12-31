#include "App.hpp"

#include <ArduinoJson.h>
#include <SD.h>

#include <algorithm>
#include <numeric>

#include "ArduinoJson/Document/StaticJsonDocument.hpp"
#include "ArduinoJson/Json/JsonSerializer.hpp"
#include "RaiiFile.hpp"
#include "common/MacAddr.hpp"
#include "common/logger.hpp"
#include "host/WebView.hpp"
#include "utils.hpp"

void App::init()
{
    if (systemInit() != Status::OK)
    {
        constexpr auto msInSecond = 1000;
        constexpr auto waitBeforeRebootSec = 5;

        logger::logErr("System will be rebooted in %ds", waitBeforeRebootSec);
        delay(waitBeforeRebootSec * msInSecond);
        ESP.restart();
    }

    m_espNow->init(
        [this](float temp, float hum, MacAddr mac, unsigned long epochTime)
        {
            auto sensorName = m_config.getSensorName(mac.str()).value_or(mac.str());
            m_readings.addReading(mac, sensorName, temp, hum, epochTime);

            auto currentReading = m_readings.lastReading(mac, sensorName);
            m_web->sendEvent(currentReading.c_str(), "newReading", millis());
        },
        m_config.getSensorUpdatePeriodMins());

    auto getSensorNames = [this]
    {
        auto &readings = m_readings.getReadingBuffers();
        std::string sensors = "[";

        bool first = true;
        for (const auto &[macAddr, readingsBuffer] : readings)
        {
            auto sensorName = m_config.getSensorName(macAddr.str()).value_or(macAddr.str());

            if (first)
            {
                sensors += "\"" + sensorName + "\"";
            }
            else
            {
                sensors += ",\"" + sensorName + "\"";
            }
        }
        sensors += ']';

        return sensors;
    };

    auto getSensorData = [this](const std::string &sensorName)
    {
        auto strmac = m_config.getSensorMac(sensorName);
        if (!strmac)
        {
            return std::string{"[]"};
        }

        auto macAddr = MacAddr::strToMac(strmac.value());
        auto readingsJson = m_readings.getReadingsAsJsonArr(macAddr, sensorName);

        logger::logInf("Sensor to download: %s, %s, %s", sensorName, strmac.value(), macAddr.str());
        auto &currentReadings = m_readings.getReadingBuffers();
        for (const auto &[macAddr, readingsBuffer] : currentReadings)
        {
            auto sensName = m_config.getSensorName(macAddr.str()).value_or("Unnamed");
        }

        return readingsJson;
    };

    m_web->startServer(getSensorNames, getSensorData);
}

void App::update()
{
}

App::Status App::systemInit()
{
    // Let the board be electrically ready before initialization
    constexpr auto waitBeforeInitializationMs = 1000;
    delay(waitBeforeInitializationMs);

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

    m_timeClient = std::make_shared<NTPClient>(m_ntpUDP);
    m_timeClient->begin();
    m_timeClient->update();

    m_espNow = std::make_unique<EspNow>(m_timeClient);
    m_web = std::make_unique<WebView>(m_config.getServerPort());

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
    std::string data = file->readString().c_str();
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
        file->print(m_config.getExampleConfig().c_str());
    }

    return Status::OK;
}

App::Status App::connectWiFi()
{
    logger::logInf("Connecting to WiFi");

    constexpr auto connectionRetriesBeforeRebootMs = 10;
    constexpr auto delayBetweenConnectionTiresMs = 1000;
    constexpr auto waitBeforeRebootMs = 1000;

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(m_config.getWifiSsid().c_str(), m_config.getWifiPass().c_str());

    uint8_t wifiConnectionTries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        wifiConnectionTries++;
        delay(delayBetweenConnectionTiresMs);
        logger::logInf(".");

        if (wifiConnectionTries >= connectionRetriesBeforeRebootMs)
        {
            logger::logErr("WiFi connection issue, reboot.");
            delay(waitBeforeRebootMs);
            ESP.restart();
        }
    }

    logger::logInf("Connected to %s IP: %s MAC: %s, channel %d", WiFi.SSID(),
                   WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str(), WiFi.channel());

    return Status::OK;
}

void App::sendEvent(float temp, float hum, MacAddr mac, unsigned long epochTime)
{
    auto sensorName = m_config.getSensorName(mac.str()).value_or(mac.str());
    std::string jsonString = utils::readingsToJsonString(temp, hum, mac, sensorName, epochTime);
    m_web->sendEvent(jsonString.c_str(), "new_readings", millis());
}
