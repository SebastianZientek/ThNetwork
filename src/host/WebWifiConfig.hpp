#pragma once

#include <ESPAsyncWebServer.h>

#include <memory>

#include "adapters/IESP32Adp.hpp"
#include "adapters/IWifi32Adp.hpp"
#include "common/logger.hpp"
#include "interfaces/IResources.hpp"
#include "webserver/IWebServer.hpp"

template <typename ConfStorageType, typename AsyncWebServerType>
class WebWifiConfig
{
public:
    WebWifiConfig(std::shared_ptr<IWifi32Adp> wifiAdp,
                  std::unique_ptr<IWebServer> server,
                  std::shared_ptr<IESP32Adp> espAdp,
                  std::unique_ptr<IResources> resources)
        : m_wifiAdp(wifiAdp)
        , m_server(std::move(server))
        , m_espAdp(espAdp)
        , m_resources(std::move(resources))
    {
    }

    void startConfiguration(std::shared_ptr<ConfStorageType> confStorage)
    {
        m_confStorage = confStorage;
        constexpr auto HTML_OK = 200;

        m_wifiAdp->softAp("TH-NETWORK");
        logger::logInf("IP addr: %s", m_wifiAdp->getSoftApIp());

        delay(m_initializationTimeMs);

        m_server->onGet("/",
                        [this](IWebRequest &request)
                        {
                            request.send(HTML_OK, "text/html", m_resources->getWifiSettingsHtml());
                        });

        m_server->onGet("/pico.min.css",
                        [this](IWebRequest &request)
                        {
                            request.send(HTML_OK, "text/css", m_resources->getPicoCss());
                        });

        m_server->onPost("/setWifi",
                         [this](IWebRequest &request)
                         {
                             logger::logDbg("Request to set wifi config");
                             auto params = request.getParams();
                             // TODO: make it safer
                             std::string ssid = params["ssid"];
                             std::string pass = params["password"];

                             logger::logDbg("Ssid: %s", ssid);

                             m_confStorage->setWifiConfig(ssid, pass);
                             m_confStorage->save();

                             request.redirect("/");
                             m_espAdp->restart();
                         });

        m_server->start();
    }

private:
    constexpr static auto m_initializationTimeMs = 1000;

    std::shared_ptr<IWifi32Adp> m_wifiAdp;
    std::unique_ptr<IWebServer> m_server;
    std::shared_ptr<ConfStorageType> m_confStorage;
    std::unique_ptr<IResources> m_resources;
    std::shared_ptr<IESP32Adp> m_espAdp;
};
