#pragma once

#include <memory>

#include "IConfStorage.hpp"
#include "Timer.hpp"
#include "WifiConfiguratorWebServer.hpp"
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
                     std::shared_ptr<IConfStorage> confStorage,
                     std::shared_ptr<WifiConfiguratorWebServer> wifiConfiguratorWebServer);

    void connect();
    Status processWifiConfiguration();

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
    std::shared_ptr<WifiConfiguratorWebServer> m_wifiConfiguratorWebServer;
    Timer m_connectionRetriesTimer{m_arduinoAdp};

    void initialize();
};
