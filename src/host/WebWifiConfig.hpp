#pragma once

#include <memory>

#include "adapters/IESP32Adp.hpp"
#include "adapters/IWifi32Adp.hpp"
#include "interfaces/IConfStorage.hpp"
#include "interfaces/IResources.hpp"
#include "webserver/IWebServer.hpp"

class WebWifiConfig
{
public:
    WebWifiConfig(const std::shared_ptr<IWifi32Adp> &wifiAdp,
                  std::unique_ptr<IWebServer> server,
                  const std::shared_ptr<IESP32Adp> &espAdp,
                  std::unique_ptr<IResources> resources);
    void startConfiguration(std::shared_ptr<IConfStorage> confStorage);

private:
    constexpr static auto m_initializationTimeMs = 1000;

    std::shared_ptr<IWifi32Adp> m_wifiAdp;
    std::unique_ptr<IWebServer> m_server;
    std::shared_ptr<IConfStorage> m_confStorage;
    std::unique_ptr<IResources> m_resources;
    std::shared_ptr<IESP32Adp> m_espAdp;
};
