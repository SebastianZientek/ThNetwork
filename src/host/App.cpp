#include "App.hpp"

#include <ArduinoJson.h>
#include <SD.h>

#include <algorithm>
#include <memory>
#include <numeric>

#include "ArduinoJson/Document/StaticJsonDocument.hpp"
#include "ArduinoJson/Json/JsonSerializer.hpp"
#include "ConfStorage.hpp"
#include "RaiiFile.hpp"
#include "WebView.hpp"
#include "common/MacAddr.hpp"
#include "common/logger.hpp"
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
            auto sensorName = m_config.getSensorName(mac.toUniqueID()).value_or("Unnamed");
            m_readingsStorage.addReading(mac.toUniqueID(), sensorName, temp, hum, epochTime);

            auto currentReading = m_readingsStorage.lastReading(mac.toUniqueID(), sensorName);
            m_web->sendEvent(currentReading.c_str(), "newReading", millis());
        },
        [this](MacAddr macAddr) {}, m_config.getSensorUpdatePeriodMins());

    auto getSensorNames = [this]
    {
        logger::logInf("getSensorNames");
        auto &readings = m_readingsStorage.getReadingBuffers();
        std::string sensors = "[";

        bool first = true;
        for (const auto &[ID, readingsBuffer] : readings)
        {
            auto sensorName = m_config.getSensorName(ID).value_or("Unnamed");

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
        logger::logInf("getSensorData");
        auto identifier = m_config.getSensorID(sensorName);
        if (!identifier)
        {
            return std::string{"[]"};
        }

        auto readingsJson = m_readingsStorage.getReadingsAsJsonArr(identifier.value(), sensorName);

        logger::logInf("Sensor to download: %s, %u", sensorName, identifier.value());
        auto &currentReadings = m_readingsStorage.getReadingBuffers();
        for (const auto &[identifier, readingsBuffer] : currentReadings)
        {
            auto sensName = m_config.getSensorName(identifier).value_or("Unnamed");
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
    m_web = std::make_unique<WebViewType>(m_config.getServerPort(), m_confStorage);

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
    {
        m_confStorage = std::make_shared<ConfStorage>();
        auto state = m_confStorage->load();
    }

    // TODO: Obsolete, will be removed while wifimanager will be in use
    {
        RaiiFile file(SD, "/config.json");
        std::string data = file->readString().c_str();
        if (!m_config.load(data))
        {
            logger::logErr("Reading config error");
            return Status::FAIL;
        }
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
        RaiiFile file(SD, "/config_example.json", FILE_WRITE);
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
    auto sensorName = m_config.getSensorName(mac.toUniqueID()).value_or("Unnamed");
    std::string jsonString = utils::readingsToJsonString(temp, hum, mac, sensorName, epochTime);
    m_web->sendEvent(jsonString.c_str(), "new_readings", millis());
}
