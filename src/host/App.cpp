#include "App.hpp"

#include <algorithm>
#include <memory>
#include <numeric>

#include "common/MacAddr.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"
#include "host/WifiConfigurator.hpp"

void App::init()
{
}

void App::update()
{
    switch (m_state)
    {
    case State::INITIALIZATION_BASIC_COMPONENTS:
        logger::init();

        {
            auto factoryReset = [this]
            {
                logger::logWrn("Reset to factory settings!");
                m_confStorage->setDefault();
                m_confStorage->save();
                m_espAdp->restart();
            };
            m_pairAndResetButton.onLongClick(3000, factoryReset);
        }

        m_wifiButton.onClick(
            [this]
            {
                // Stop all servives and host wifi configurator
                // wifiSettingsMode();
                m_state = State::HOSTING_WIFI_CONFIGURATION;
            });

        m_ledIndicator->switchOn(false);

        m_state = State::LOADING_CONFIGURATION;
        break;

    case State::LOADING_CONFIGURATION:
        logger::logDbg("Loading configuration");

        if (auto status = m_confStorage->load(); status == ConfStorage::State::FAIL)
        {
            logger::logWrn("Configuration file not exists, using default values");
        }

        m_wifiConfigurator.connect();
        m_state = State::CONNECTING_TO_WIFI;
        break;

    case State::CONNECTING_TO_WIFI:
        if (auto status = m_wifiConfigurator.processWifiConfiguration();
            status == WiFiConfigurator::Status::CONNECTED)
        {
            m_state = State::STARTING_SERVERS;
        }
        else if (status == WiFiConfigurator::Status::CONNECTION_FAILURE)
        {
            m_state = State::ERROR_REBOOTING;
        }
        else if (status == WiFiConfigurator::Status::CONFIGURATION_PAGE_HOSTED)
        {
            startWebWifiConfiguration();
            m_state = State::HOSTING_WIFI_CONFIGURATION;
        }

        break;

    case State::HOSTING_WIFI_CONFIGURATION:
        break;

    case State::STARTING_SERVERS:
        logger::logDbg("Starting servers");

        m_timeClient->begin();
        m_timeClient->update();

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
                return m_readingsStorage.getReadingsAsJsonStr(identifier);
            };

            m_webPageMain->startServer(getSensorData);
        }

        m_pairAndResetButton.onClick(
            [this]
            {
                m_pairingManager->enablePairingForPeriod();
            });

        m_state = State::RUNNING;
        break;

    case State::RUNNING:
        break;
    }

    m_pairAndResetButton.update();
    m_wifiButton.update();
    m_wifiConfigurationTimer.update();

    m_pairingManager->update();
    m_ledIndicator->update();
}

void App::startWebWifiConfiguration()
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
    m_webWifiConfig->startServer();

    m_wifiConfigurationTimer.setCallback(
        [this]
        {
            logger::logInf("Wifi configuration timeout. Reboot...");
            m_espAdp->restart();
        });
    m_wifiConfigurationTimer.start(m_wifiConfigServerTimeoutMillis);
}
