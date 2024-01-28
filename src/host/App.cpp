#include "App.hpp"

#include <SD.h>

#include <algorithm>
#include <memory>
#include <numeric>

#include "ConfStorage.hpp"
#include "RaiiFile.hpp"
#include "WebView.hpp"
#include "WiFi.h"
#include "common/MacAddr.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"
#include "esp32-hal-gpio.h"
#include "utils.hpp"

void App::init()
{
    if (auto initStatus = systemInit(); initStatus == Status::FAIL)
    {
        constexpr auto msInSecond = 1000;
        constexpr auto waitBeforeRebootSec = 5;

        logger::logErr("System will be rebooted in %ds", waitBeforeRebootSec);
        delay(waitBeforeRebootSec * msInSecond);
        ESP.restart();
    }
    else if (initStatus == Status::WIFI_CONFIGURATION_NEEDED)
    {
        wifiSettingsMode();
    }
    else
    {
        m_espNow->init(
            [this](float temp, float hum, IDType identifier, unsigned long epochTime)
            {
                m_readingsStorage.addReading(identifier, temp, hum, epochTime);

                logger::logInf("New reading sending");
                auto reading = m_readingsStorage.getLastReadingAsJson(identifier);
                m_web->sendEvent(reading.c_str(), "newReading", millis());
            },
            [this](IDType identifier) {}, m_confStorage->getSensorUpdatePeriodMins());

        auto getSensorData = [this](const std::size_t &identifier)
        {
            logger::logInf("getSensorData");
            auto data = m_readingsStorage.getReadingsAsJsonArr(identifier);
            return data;
        };

        m_web->startServer(getSensorData);
    }
}

void App::update()
{
    if (m_mode != Mode::WIFI_SETTINGS && isWifiButtonPressed())
    {
        wifiSettingsMode();
    }
}

App::Status App::systemInit()
{
    // Let the board be electrically ready before initialization
    constexpr auto waitBeforeInitializationMs = 1000;
    delay(waitBeforeInitializationMs);
    setupWifiButton();

    logger::init();
    if (auto status = initSD(); status != Status::OK)
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
    m_web = std::make_unique<WebViewType>(m_confStorage);

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
    m_confStorage = std::make_shared<ConfStorage>();
    auto state = m_confStorage->load();

    if (state == ConfStorage::State::OK)
    {
        return Status::OK;
    }
    else
    {
        return Status::FAIL;
    }
}

App::Status App::connectWiFi()
{
    logger::logInf("Connecting to WiFi");

    constexpr auto connectionRetriesBeforeRebootMs = 10;
    constexpr auto delayBetweenConnectionTiresMs = 1000;
    constexpr auto waitBeforeRebootMs = 1000;

    WiFi.mode(WIFI_AP_STA);

    auto wifiConfig = m_confStorage->getWifiConfig();
    if (!wifiConfig)
    {
        return Status::WIFI_CONFIGURATION_NEEDED;
    }

    auto [ssid, pass] = wifiConfig.value();
    WiFi.begin(ssid.c_str(), pass.c_str());

    uint8_t wifiConnectionTries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (isWifiButtonPressed())
        {
            return Status::WIFI_CONFIGURATION_NEEDED;
        }

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

void App::wifiSettingsMode()
{
    logger::logInf("Wifi settings mode");
    m_mode = Mode::WIFI_SETTINGS;
    if (m_espNow)
    {
        m_espNow->deinit();
    }
    if (m_web)
    {
        m_web->stopServer();
    }
    WiFi.disconnect();

    m_webWifiConfig = std::make_unique<WebWifiConfigType>();
    m_webWifiConfig->startConfiguration(m_confStorage);
}

void App::setupWifiButton()
{
    pinMode(21, INPUT_PULLUP);
}

bool App::isWifiButtonPressed()
{
    return digitalRead(21) == LOW;
}
