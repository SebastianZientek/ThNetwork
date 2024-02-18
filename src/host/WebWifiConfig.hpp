#pragma once

#include <ESPAsyncWebServer.h>

#include <memory>

#include "adapters/IESP32Adp.hpp"
#include "adapters/IWifi32Adp.hpp"
#include "common/logger.hpp"
#include "interfaces/IResources.hpp"

template <typename ConfStorageType, typename AsyncWebServerType>
class WebWifiConfig
{
public:
    WebWifiConfig(std::shared_ptr<IWifi32Adp> wifiAdp,
                  std::shared_ptr<IESP32Adp> espAdp,
                  std::unique_ptr<IResources> resources)
        : m_server(80)
        , m_wifiAdp(wifiAdp)
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

        delay(1000);

        m_server.on("/", HTTP_GET,
                    [this](AsyncWebServerRequest *request)
                    {
                        request->send_P(HTML_OK, "text/html", m_resources->getWifiSettingsHtml());
                    });

        m_server.on("/setWifi", HTTP_POST,
                    [this](AsyncWebServerRequest *request)
                    {
                        auto params = request->params();
                        std::string ssid{};
                        std::string pass{};
                        for (int i = 0; i < params; i++)
                        {
                            AsyncWebParameter *param = request->getParam(i);
                            if (param->name() == "ssid")
                            {
                                ssid = param->value().c_str();
                            }
                            else if (param->name() == "password")
                            {
                                pass = param->value().c_str();
                            }
                        }

                        m_confStorage->setWifiConfig(ssid, pass);
                        m_confStorage->save();

                        request->redirect("/");

                        m_espAdp->restart();
                    });

        m_server.begin();
    }

private:
    AsyncWebServerType m_server;
    std::shared_ptr<IWifi32Adp> m_wifiAdp{};
    std::shared_ptr<ConfStorageType> m_confStorage;
    std::unique_ptr<IResources> m_resources;
    std::shared_ptr<IESP32Adp> m_espAdp;
};
