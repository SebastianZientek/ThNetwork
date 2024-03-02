#pragma once

#include <memory>

#include "IConfStorage.hpp"
#include "IResources.hpp"
#include "adapters/IESP32Adp.hpp"
#include "adapters/IWifi32Adp.hpp"
#include "webserver/IWebServer.hpp"

class WifiConfiguratorWebServer
{
public:
    WifiConfiguratorWebServer(const std::shared_ptr<IWifi32Adp> &wifiAdp,
                              std::unique_ptr<IWebServer> server,
                              const std::shared_ptr<IESP32Adp> &espAdp,
                              std::unique_ptr<IResources> resources,
                              std::shared_ptr<IConfStorage> confStorage);
    void startServer();

private:
    constexpr static auto m_initializationTimeMs = 1000;

    std::shared_ptr<IWifi32Adp> m_wifiAdp;
    std::unique_ptr<IWebServer> m_server;
    std::shared_ptr<IConfStorage> m_confStorage;
    std::unique_ptr<IResources> m_resources;
    std::shared_ptr<IESP32Adp> m_espAdp;
};
