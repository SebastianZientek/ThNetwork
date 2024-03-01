#pragma once

#include <memory>

#include "IConfStorage.hpp"
#include "Timer.hpp"
#include "adapters/IArduino32Adp.hpp"
#include "adapters/IWifi32Adp.hpp"
#include "common/logger.hpp"

class WiFiConfigurator
{
public:
    enum class Status
    {
        CONNECTION_ONGOING,
        CONNECTED,
        CONFIGURATION_PAGE_HOSTED,
        CONNECTION_FAILURE,
        NOT_INITIALIZED
    };

    WiFiConfigurator(std::shared_ptr<IArduino32Adp> arduinoAdp,
                     std::shared_ptr<IWifi32Adp> wifiAdp,
                     std::shared_ptr<IConfStorage> confStorage)
        : m_arduinoAdp(arduinoAdp)
        , m_wifiAdp(wifiAdp)
        , m_confStorage(confStorage)
    {
    }

    void connect()
    {
        m_mode = Mode::INITIALIZE;
    }

    Status processWifiConfiguration()
    {
        switch (m_mode)
        {
        case Mode::NOT_INITIALIZED:
            return Status::NOT_INITIALIZED;
            break;

        case Mode::INITIALIZE:
            logger::logDbg("WifiConfigurator Mode::INITIALIZE");
            initialize();
            break;

        case Mode::PROCESSING_CONNECTION:
            m_connectionRetriesTimer.update();
            return Status::CONNECTION_ONGOING;
            // TODO: Reconnect if connection lost
            break;

        case Mode::HOST_WIFI_CONFIGURATOR:
            logger::logDbg("WifiConfigurator Mode::HOST_WIFI_CONFIGURATOR");
            hostWifiConfiguration();
            return Status::CONFIGURATION_PAGE_HOSTED;
            break;

        case Mode::CONNECTED:
            logger::logDbg("WifiConfigurator Mode::CONNECTED");
            return Status::CONNECTED;
            break;

        case Mode::ERROR:
            return Status::CONNECTION_FAILURE;
            break;
        }

        return Status::NOT_INITIALIZED;
    }

    void hostWifiConfiguration()
    {
    }

private:
    enum class Mode
    {
        NOT_INITIALIZED,
        ERROR,
        INITIALIZE,
        HOST_WIFI_CONFIGURATOR,
        PROCESSING_CONNECTION,
        CONNECTED,
    };

    constexpr static auto m_delayBetweenConnectionRetiresMs = 1000;
    constexpr static auto m_maxConnectionRetries = 10;

    Mode m_mode{Mode::NOT_INITIALIZED};
    std::shared_ptr<IArduino32Adp> m_arduinoAdp;
    std::shared_ptr<IWifi32Adp> m_wifiAdp;
    std::shared_ptr<IConfStorage> m_confStorage;
    Timer m_connectionRetriesTimer{m_arduinoAdp};

    void initialize()
    {
        logger::logInf("Connecting to WiFi");
        m_wifiAdp->setMode(IWifi32Adp::Mode::AP_STA);

        auto wifiConfig = m_confStorage->getWifiConfig();
        if (!wifiConfig)
        {
            logger::logWrn("No wifi configuration!");
            m_mode = Mode::HOST_WIFI_CONFIGURATOR;
        };

        auto [ssid, pass] = wifiConfig.value();
        m_wifiAdp->init(ssid, pass);

        auto connectToWifi = [this, retries = 0]() mutable
        {
            if (retries++, retries > m_maxConnectionRetries)
            {
                m_connectionRetriesTimer.stop();
                m_mode = Mode::ERROR;
                // CONNECTION ERROR, STOP TRYING
            }

            logger::logInf(".");
            if (m_wifiAdp->getStatus() == IWifi32Adp::Status::CONNECTED)
            {
                m_mode = Mode::CONNECTED;
                logger::logInf("Connected to %s IP: %s MAC: %s, channel %d", m_wifiAdp->getSsid(),
                               m_wifiAdp->getLocalIp(), m_wifiAdp->getMacAddr(),
                               m_wifiAdp->getChannel());
            }
        };

        connectToWifi();
        m_connectionRetriesTimer.setCallback(connectToWifi);
        m_connectionRetriesTimer.start(m_delayBetweenConnectionRetiresMs, true);

        m_mode = Mode::PROCESSING_CONNECTION;
    }
};
