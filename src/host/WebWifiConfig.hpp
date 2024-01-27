#pragma once

#include <ESPAsyncWebServer.h>

#include <memory>

#include "common/logger.hpp"

extern "C"
{
    extern const char gWifiSettingsHtmlData[];
}

template <typename ConfStorageType, typename AsyncWebServerType>
class WebWifiConfig
{
public:
    WebWifiConfig()
        : m_server(80)
    {
    }

    void startConfiguration(std::shared_ptr<ConfStorageType> confStorage)
    {
        m_confStorage = confStorage;
        constexpr auto HTML_OK = 200;

        WiFi.softAP("TH-NETWORK", nullptr);
        IPAddress IP = WiFi.softAPIP();

        logger::logInf("IP addr: %s", IP.toString().c_str());

        delay(1000);

        m_server.on("/", HTTP_GET,
                    [this](AsyncWebServerRequest *request)
                    { request->send_P(HTML_OK, "text/html", gWifiSettingsHtmlData); });

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

                        ESP.restart();
                    });

        m_server.begin();
    }

private:
    AsyncWebServerType m_server;
    std::shared_ptr<ConfStorageType> m_confStorage;
};
