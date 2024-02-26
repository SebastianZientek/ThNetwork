#include "App.hpp"

#include <algorithm>
#include <memory>
#include <numeric>

#include "ConfStorage.hpp"
#include "EspNowPairingManager.hpp"
#include "LedIndicator.hpp"
#include "Resources.hpp"
#include "WebPageMain.hpp"
#include "adapters/Arduino32Adp.hpp"
#include "adapters/ESP32Adp.hpp"
#include "adapters/EspNow32Adp.hpp"
#include "adapters/SPIFS32Adp.hpp"
#include "adapters/Wifi32Adp.hpp"
#include "common/MacAddr.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"
#include "webserver/WebServer.hpp"

void App::init()
{
    m_internalFS = std::make_shared<SPIFFS32Adp>();
    m_wifiAdp = std::make_shared<Wifi32Adp>();
    m_arduinoAdp = std::make_shared<Arduino32Adp>();
    m_confStorage = std::make_shared<ConfStorage>(m_internalFS, "/config.json");
    m_espAdp = std::make_shared<ESP32Adp>();
    m_ledIndicator = std::make_shared<LedIndicator>(m_arduinoAdp, m_ledIndicatorPin);
    m_pairingManager
        = std::make_unique<EspNowPairingManager>(m_confStorage, m_arduinoAdp, m_ledIndicator);
    m_espNow = std::make_unique<EspNowServer>(std::make_unique<EspNow32Adp>(), m_pairingManager,
                                              m_wifiAdp);
    m_timeClient = std::make_shared<NTPClient>(m_ntpUDP);
    m_webPageMain = std::make_unique<WebPageMain>(m_arduinoAdp, std::make_unique<WebServer>(),
                                                  std::make_unique<Resources>(), m_confStorage);

    if (auto initState = systemInit(); initState == State::FAIL)
    {
        constexpr auto msInSecond = 1000;
        constexpr auto waitBeforeRebootSec = 5;

        logger::logErr("System not initialized properly. Reboot in %ds", waitBeforeRebootSec);
        delay(waitBeforeRebootSec * msInSecond);
        m_espAdp->restart();
    }
    else if (initState == State::WIFI_CONFIGURATION_NEEDED)
    {
        wifiSettingsMode();
    }
    else
    {
        auto newReadingCallback = [this](float temp, float hum, IDType identifier)
        {
            m_readingsStorage.addReading(identifier, temp, hum, m_timeClient->getEpochTime());

            auto reading = m_readingsStorage.getLastReadingAsJsonStr(identifier);
            m_webPageMain->sendEvent(reading.c_str(), "newReading", m_arduinoAdp->millis());
        };

        m_espNow->init(newReadingCallback, m_confStorage->getSensorUpdatePeriodMins());

        auto getSensorData = [this](const std::size_t &identifier)
        {
            auto data = m_readingsStorage.getReadingsAsJsonStr(identifier);
            return data;
        };

        m_webPageMain->startServer(getSensorData);
    }

    logger::logInf("System initialized");
}

void App::update()
{
    static decltype(m_arduinoAdp->millis()) wifiModeStartTime = 0;
    if (m_mode != Mode::WIFI_SETTINGS && isWifiButtonPressed())
    {
        wifiModeStartTime = m_arduinoAdp->millis();
        wifiSettingsMode();
    }

    if (m_mode == Mode::WIFI_SETTINGS
        && m_arduinoAdp->millis() > wifiConfigServerTimeoutMillis + wifiModeStartTime)
    {
        logger::logInf("Wifi configuration timeout. Reboot...");
        m_espAdp->restart();
    }

    if (isPairButtonPressed())
    {
        m_pairingManager->enablePairingForPeriod();
    }

    m_pairingManager->update();
    m_ledIndicator->update();
}

App::State App::systemInit()
{
    // Let the board be electrically ready before initialization
    constexpr auto waitBeforeInitializationMs = 1000;
    delay(waitBeforeInitializationMs);
    logger::init();

    setupButtons();
    m_ledIndicator->switchOn(false);

    if (auto state = initConfig(); state != State::OK)
    {
        return state;
    }
    if (auto state = connectWiFi(); state != State::OK)
    {
        return state;
    }

    m_timeClient->begin();
    m_timeClient->update();

    return State::OK;
}

App::State App::initConfig()
{
    auto state = m_confStorage->load();
    if (state == ConfStorage::State::FAIL)
    {
        logger::logWrn("Configuration file not exists, using default values");
    }

    return State::OK;
}

App::State App::connectWiFi()
{
    logger::logInf("Connecting to WiFi");

    constexpr auto connectionRetriesBeforeRebootMs = 10;
    constexpr auto delayBetweenConnectionTiresMs = 1000;
    constexpr auto waitBeforeRebootMs = 1000;

    m_wifiAdp->setMode(Wifi32Adp::Mode::AP_STA);

    auto wifiConfig = m_confStorage->getWifiConfig();
    if (!wifiConfig)
    {
        logger::logWrn("No wifi configuration!");
        return State::WIFI_CONFIGURATION_NEEDED;
    }

    auto [ssid, pass] = wifiConfig.value();
    m_wifiAdp->init(ssid, pass);

    uint8_t wifiConnectionTries = 0;
    while (m_wifiAdp->getStatus() != Wifi32Adp::Status::CONNECTED)
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
            logger::logWrn("WiFi connection issue, reboot.");
            delay(waitBeforeRebootMs);
            m_espAdp->restart();
        }
    }

    logger::logInf("Connected to %s IP: %s MAC: %s, channel %d", m_wifiAdp->getSsid(),
                   m_wifiAdp->getLocalIp(), m_wifiAdp->getMacAddr(), m_wifiAdp->getChannel());

    return State::OK;
}

void App::wifiSettingsMode()
{
    logger::logInf("Wifi configuration mode");
    m_ledIndicator->switchOn(true);

    m_mode = Mode::WIFI_SETTINGS;
    if (m_espNow)
    {
        m_espNow->deinit();
    }
    if (m_webPageMain)
    {
        m_webPageMain->stopServer();
    }
    m_wifiAdp->disconnect();

    m_webWifiConfig = std::make_unique<WebWifiConfigType>(m_wifiAdp, std::make_unique<WebServer>(),
                                                          m_espAdp, std::make_unique<Resources>());
    m_webWifiConfig->startConfiguration(m_confStorage);
}

void App::setupButtons()
{
    m_arduinoAdp->pinMode(wifiButton, Arduino32Adp::Mode::PIN_INPUT_PULLUP);
    m_arduinoAdp->pinMode(pairButton, Arduino32Adp::Mode::PIN_INPUT_PULLUP);
}

bool App::isWifiButtonPressed()
{
    return m_arduinoAdp->digitalRead(wifiButton) == false;
}

bool App::isPairButtonPressed()
{
    return m_arduinoAdp->digitalRead(pairButton) == false;
}
