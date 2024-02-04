#include "App.hpp"

#include <SPIFFS.h>

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
    if (auto initState = systemInit(); initState == State::FAIL)
    {
        constexpr auto msInSecond = 1000;
        constexpr auto waitBeforeRebootSec = 5;

        logger::logErr("System will be rebooted in %ds", waitBeforeRebootSec);
        delay(waitBeforeRebootSec * msInSecond);
        ESP.restart();
    }
    else if (initState == State::WIFI_CONFIGURATION_NEEDED)
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

        // TODO: STUB, remove after implementation ready
        m_confStorage->addSensor(2506682365, "Some sensor name");
    }
}

void App::update()
{
    if (m_mode != Mode::WIFI_SETTINGS && isWifiButtonPressed())
    {
        wifiSettingsMode();
    }
}

App::State App::systemInit()
{
    // Let the board be electrically ready before initialization
    constexpr auto waitBeforeInitializationMs = 1000;
    delay(waitBeforeInitializationMs);
    setupWifiButton();

    logger::init();
    if (auto state = initConfig(); state != State::OK)
    {
        return state;
    }
    if (auto state = connectWiFi(); state != State::OK)
    {
        return state;
    }

    m_timeClient = std::make_shared<NTPClient>(m_ntpUDP);
    m_timeClient->begin();
    m_timeClient->update();

    m_espNow = std::make_unique<EspNow>(m_timeClient);
    m_web = std::make_unique<WebViewType>(m_confStorage);

    return State::OK;
}

App::State App::initConfig()
{
    SPIFFS.begin(true);
    RaiiFile configFile(SPIFFS.open("/config.json"));

    m_confStorage = std::make_shared<ConfStorage>();
    auto state = m_confStorage->load(configFile);

    if (state == ConfStorage::State::FAIL)
    {
        logger::logWrn("File not exists, setting and saving defaults");
        m_confStorage->setDefault();
        if (m_confStorage->save(configFile) == ConfStorage::State::FAIL)
        {
            logger::logErr("Can't save settings");
            return State::FAIL;
        }
    }

    return State::OK;
}

App::State App::connectWiFi()
{
    logger::logInf("Connecting to WiFi");

    constexpr auto connectionRetriesBeforeRebootMs = 10;
    constexpr auto delayBetweenConnectionTiresMs = 1000;
    constexpr auto waitBeforeRebootMs = 1000;

    WiFi.mode(WIFI_AP_STA);

    auto wifiConfig = m_confStorage->getWifiConfig();
    if (!wifiConfig)
    {
        logger::logWrn("No wifi configuration!");
        return State::WIFI_CONFIGURATION_NEEDED;
    }

    auto [ssid, pass] = wifiConfig.value();
    WiFi.begin(ssid.c_str(), pass.c_str());

    uint8_t wifiConnectionTries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (isWifiButtonPressed())
        {
            return State::WIFI_CONFIGURATION_NEEDED;
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

    return State::OK;
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
