#pragma once

#include <functional>
#include <memory>
#include <string>

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
        NOT_INITIALIZED,
        CONNECTION_ONGOING,
        CONNECTION_SUCCESS,
        CONNECTION_FAILURE
    };

    WiFiConfigurator(const std::shared_ptr<IArduino32Adp> &arduinoAdp,
                     const std::shared_ptr<IWifi32Adp> &wifiAdp);

    void connect(const std::string &ssid, const std::string &pass);
    Status status();
    void update();

private:
    constexpr static auto m_delayBetweenConnectionRetiresMs = 1000;
    constexpr static auto m_maxConnectionRetries = 10;

    std::shared_ptr<IArduino32Adp> m_arduinoAdp;
    std::shared_ptr<IWifi32Adp> m_wifiAdp;

    Timer m_connectionRetriesTimer{m_arduinoAdp};
    Status m_status{Status::NOT_INITIALIZED};
    std::string m_ssid{};
    std::string m_pass{};

    void initialize();
};
