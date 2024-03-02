#pragma once

#include <functional>
#include <memory>

#include "IConfStorage.hpp"
#include "IResources.hpp"
#include "adapters/IArduino32Adp.hpp"
#include "adapters/IESP32Adp.hpp"
#include "adapters/IWifi32Adp.hpp"
#include "webserver/IWebServer.hpp"

class WifiConfiguratorWebServer
{
public:
    using OnConfiguredClbk = std::function<void(const std::string &ssid, const std::string &pass)>;

    WifiConfiguratorWebServer(const std::shared_ptr<IWifi32Adp> &wifiAdp,
                              std::shared_ptr<IWebServer> server,
                              std::unique_ptr<IResources> resources,
                              const std::shared_ptr<IArduino32Adp> &arduinoAdp);
    void startServer(const OnConfiguredClbk &onConfiguredClbk);

private:
    constexpr static auto m_initializationTimeMs = 1000;

    std::shared_ptr<IWifi32Adp> m_wifiAdp;
    std::shared_ptr<IWebServer> m_server;
    std::unique_ptr<IResources> m_resources;
    std::shared_ptr<IArduino32Adp> m_arduinoAdp;
};
