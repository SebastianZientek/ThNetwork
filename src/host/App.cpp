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
    logger::init();
    initConfiguration();
    initPeripherals();
    initWifi();
}

void App::update()
{
    switch (m_mode)
    {
    case Mode::INITIALIZATION:
        break;
    case Mode::WAITING_FOR_WIFI:
        if (auto wifiStatus = m_wifiConfigurator.status();
            wifiStatus == WiFiConfigurator::Status::CONNECTION_SUCCESS)
        {
            m_mode = Mode::STARTING_SERVICES;
        }
        else if (wifiStatus == WiFiConfigurator::Status::CONNECTION_FAILURE)
        {
            constexpr auto timeoutMs = 3000;
            logger::logWrn("Can't connect to wifi, reboot in 3s");
            m_arduinoAdp->delay(timeoutMs);
            m_espAdp->restart();
        }
        break;
    case Mode::HOSTING_WIFI_CONFIGURATOR:
        break;
    case Mode::STARTING_SERVICES:
        startServices();
        m_mode = Mode::NORMAL_OPERATION;
        break;
    case Mode::NORMAL_OPERATION:
        break;
    }

    m_pairAndResetButton.update();
    m_wifiButton.update();
    m_wifiConfigurator.update();
    m_wifiConfigurationTimer.update();

    m_pairingManager->update();
    m_ledIndicator->update();
}

void App::startWebWifiConfiguration()
{
    logger::logInf("Wifi configuration mode");
    m_mode = Mode::HOSTING_WIFI_CONFIGURATOR;
    m_ledIndicator->switchOn(true);

    if (m_espNow)
    {
        m_espNow->deinit();
    }
    if (m_webPageMain)
    {
        m_webPageMain->stopServer();
    }
    m_wifiAdp->disconnect();
    m_webWifiConfig->startServer(
        [this](const std::string &ssid, const std::string &pass)
        {
            m_confStorage->setWifiConfig(ssid, pass);
            m_confStorage->save();
            m_espAdp->restart();
        });

    m_wifiConfigurationTimer.setCallback(
        [this]
        {
            logger::logInf("Wifi configuration timeout. Reboot...");
            m_espAdp->restart();
        });
    m_wifiConfigurationTimer.start(m_wifiConfigServerTimeoutMillis);
}

void App::initConfiguration()
{
    logger::logDbg("Loading configuration");
    if (auto status = m_confStorage->load(); status == ConfStorage::State::FAIL)
    {
        logger::logWrn("Configuration file not exists, using default values");
    }
}

void App::initPeripherals()
{
    auto factoryReset = [this]
    {
        logger::logWrn("Reset to factory settings!");
        m_confStorage->setDefault();
        m_confStorage->save();
        m_espAdp->restart();
    };
    m_pairAndResetButton.onLongClick(3000, factoryReset);
    m_wifiButton.onClick([this] { startWebWifiConfiguration(); });
    m_ledIndicator->switchOn(false);
}

void App::initWifi()
{
    auto wifiConfig = m_confStorage->getWifiConfig();
    if (wifiConfig.has_value())
    {
        auto [ssid, pass] = wifiConfig.value();
        m_wifiConfigurator.connect(ssid, pass);
        m_mode = Mode::WAITING_FOR_WIFI;
    }
    else
    {
        startWebWifiConfiguration();
    }
}

void App::startServices()
{
    logger::logDbg("Starting services");

    m_timeClient->begin();
    m_timeClient->update();

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
    m_webPageMain->startServer([this](const std::size_t &identifier)
                               { return m_readingsStorage.getReadingsAsJsonStr(identifier); });
    m_pairAndResetButton.onClick([this] { m_pairingManager->enablePairingForPeriod(); });
}
