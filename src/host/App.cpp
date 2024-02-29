#include "App.hpp"

#include <algorithm>
#include <memory>
#include <numeric>

#include "common/MacAddr.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"

void App::init()
{
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
        m_webPageMain = std::make_unique<WebPageMain>(
            m_arduinoAdp, std::make_shared<WebServer>(m_confStorage->getServerPort()),
            std::make_unique<Resources>(), m_confStorage);

        auto newReadingCallback = [this](float temp, float hum, IDType identifier)
        {
            m_readingsStorage.addReading(identifier, temp, hum, m_timeClient->getEpochTime());

            auto reading = m_readingsStorage.getLastReadingAsJsonStr(identifier);
            m_webPageMain->sendEvent(reading.c_str(), "newReading", m_arduinoAdp->millis());
        };

        m_espNow->init(newReadingCallback);

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

    static decltype(m_arduinoAdp->millis()) resetStarted = 0;
    if (isWifiButtonPressed())
    {
        if (resetStarted == 0)
        {
            resetStarted = m_arduinoAdp->millis();
        }
        else if (m_arduinoAdp->millis() - resetStarted > m_resetToFactorySettings)
        {
            logger::logWrn("Reset to factory settings!");
            m_confStorage->setDefault();
            m_confStorage->save();
            m_espAdp->restart();
        }
    }
    else
    {
        resetStarted = 0;
    }

    if (m_mode == Mode::WIFI_SETTINGS
        && m_arduinoAdp->millis() > m_wifiConfigServerTimeoutMillis + wifiModeStartTime)
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
        delay(m_delayBetweenConnectionRetiresMs);
        logger::logInf(".");

        if (wifiConnectionTries >= m_connectionRetriesBeforeRebootMs)
        {
            logger::logWrn("WiFi connection issue, reboot.");
            delay(m_onErrorWaitBeforeRebootMs);
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
    m_webWifiConfig->startConfiguration(m_confStorage);
}

void App::setupButtons()
{
    m_arduinoAdp->pinMode(m_wifiButton, Arduino32Adp::Mode::PIN_INPUT_PULLUP);
    m_arduinoAdp->pinMode(m_pairButton, Arduino32Adp::Mode::PIN_INPUT_PULLUP);
}

bool App::isWifiButtonPressed()
{
    return m_arduinoAdp->digitalRead(m_wifiButton) == false;
}

bool App::isPairButtonPressed()
{
    return m_arduinoAdp->digitalRead(m_pairButton) == false;
}
