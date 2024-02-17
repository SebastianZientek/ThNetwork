#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>

#include "adapters/IArduino32Adp.hpp"
#include "common/logger.hpp"
#include "interfaces/IConfStorage.hpp"
#include "interfaces/IResources.hpp"
#include "webserver/WebServer.hpp"

class WebPageMain
{
    using GetSensorDataCb = std::function<std::string(const std::size_t &)>;

public:
    WebPageMain(std::shared_ptr<IArduino32Adp> arduinoAdp,
                std::unique_ptr<IWebServer> webServer,
                std::unique_ptr<IResources> resources,
                std::shared_ptr<IConfStorage> confStorage)
        : m_arduinoAdp(arduinoAdp)
        , m_server(std::move(webServer))
        , m_confStorage(confStorage)
        , m_resources(std::move(resources))
    {
    }

    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0)

    {
        logger::logInf("Send event %s, %lu", event, identifier);
        m_server->sendEvent(message, event, identifier, reconnect);
    }

    void startServer(const GetSensorDataCb &getSensorDataCb)
    {
        m_getSensorDataCb = getSensorDataCb;

        constexpr auto HTML_OK = 200;
        constexpr auto HTML_UNAUTH = 401;
        constexpr auto HTML_NOT_FOUND = 404;
        constexpr auto RECONNECT_TIMEOUT = 10000;

        auto auth = [this](IWebRequest &request)
        {
            auto credentials = m_confStorage->getAdminCredentials();
            if (!credentials.has_value())
            {
                return false;
            }

            auto [user, passwd] = credentials.value();
            return request.authenticate(user.c_str(), passwd.c_str());
        };

        m_server->onGet("/",
                        [this](IWebRequest &request)
                        {
                            request.send(HTML_OK, "text/html", m_resources->getIndexHtml());
                        });

        m_server->onGet("/admin",
                        [this, auth](IWebRequest &request)
                        {
                            if (!auth(request))
                            {
                                return request.requestAuthentication();
                            }
                            request.send(HTML_OK, "text/html", m_resources->getAdminHtml());
                        });

        m_server->onPost("/setCredentials",
                         [this](IWebRequest &request)
                         {
                             for (auto [key, value] : request.getParams())
                             {
                             }

                             request.send(HTML_OK, "text/html", m_resources->getAdminHtml());
                         });

        m_server->onGet("/logout",
                        [](IWebRequest &request)
                        {
                            request.send(HTML_UNAUTH);
                        });

        m_server->onGet("/favicon.ico",
                        [this](IWebRequest &request)
                        {
                            request.send(HTML_OK, "image/png", m_resources->getFavicon(),
                                         m_resources->getFaviconSize());
                        });

        m_server->onGet("/microChart.js",
                        [this](IWebRequest &request)
                        {
                            request.send(HTML_OK, "application/javascript",
                                         m_resources->getMicroChart());
                        });

        m_server->onGet("/sensorIDsToNames",
                        [this](IWebRequest &request)
                        {
                            auto sensorsMappingJsonStr = m_confStorage->getSensorsMapping();
                            logger::logInf("sensorIDsToNames %s", sensorsMappingJsonStr);
                            request.send(HTML_OK, "application/json",
                                         sensorsMappingJsonStr.c_str());
                        });

        m_server->onGet("/configuration",
                        [this, auth](IWebRequest &request)
                        {
                            if (!auth(request))
                            {
                                request.send(HTML_UNAUTH);
                            }

                            auto config = m_confStorage->getConfigWithoutCredentials();
                            request.send(HTML_OK, "application/json", config.c_str());
                        });

        m_server->onGet("/sensorData",
                        [this](IWebRequest &request)
                        {
                            auto params = request.getParams();
                            if (params.find("identifier") != params.end())
                            {
                                // TODO: make it safer
                                auto identifier = std::stoull(params["identifier"]);
                                request.send(HTML_OK, "application/json",
                                             m_getSensorDataCb(identifier).c_str());
                            }
                            else
                            {
                                request.send(HTML_NOT_FOUND);
                            }
                        });

        m_server->setupEventsSource(
            "/events",
            [this](IEventSrcClient &client)
            {
                logger::logInf("Client connected");
                if (client.lastId() != 0)
                {
                    logger::logInf("Client reconnected, last ID: %u\n", client.lastId());
                }
                client.send("init", nullptr, m_arduinoAdp->millis(), RECONNECT_TIMEOUT);
            });

        m_server->start();
    }

    void stopServer()
    {
        m_server->stop();
    }

private:
    std::shared_ptr<IArduino32Adp> m_arduinoAdp;
    std::unique_ptr<IWebServer> m_server;
    std::unique_ptr<IResources> m_resources;
    std::shared_ptr<IConfStorage> m_confStorage;

    GetSensorDataCb m_getSensorDataCb;
};
