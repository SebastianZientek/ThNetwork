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
        logger::logDbg("Send event %s, %lu", event, identifier);
        m_server->sendEvent(message, event, identifier, reconnect);
    }

    void startServer(const GetSensorDataCb &getSensorDataCb)
    {
        m_getSensorDataCb = getSensorDataCb;

        constexpr auto HTML_OK = 200;
        constexpr auto HTML_BAD_REQ = 400;
        constexpr auto HTML_UNAUTH = 401;
        constexpr auto HTML_NOT_FOUND = 404;
        constexpr auto HTML_INTERNAL_ERR = 500;
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
                         [this, auth](IWebRequest &request, std::string body)
                         {
                             if (!auth(request))
                             {
                                 return request.requestAuthentication();
                             }

                             auto credentials = nlohmann::json::parse(body);
                            request.send(HTML_BAD_REQ);
                             if (credentials["password"] != credentials["re_password"]
                                 || credentials["password"].empty()
                                 || credentials["username"].empty())
                             {
                                 request.send(HTML_BAD_REQ);
                                 return;
                             }

                             m_confStorage->setAdminCredentials(credentials["username"],
                                                                credentials["password"]);
                             m_confStorage->save();

                             request.send(HTML_OK, "text/html", m_resources->getAdminHtml());
                         });

        m_server->onPost("/updateSensorsMapping",
                         [this](IWebRequest &request, std::string body)
                         {
                             logger::logDbg("Update configuration %s", body);
                             auto sensorsMapping = nlohmann::json::parse(body);

                             for (auto sensor : sensorsMapping.items())
                             {
                                 auto name = std::string(sensor.value());
                                 auto id = std::stoull(sensor.key());

                                 m_confStorage->addSensor(id, name);
                                 m_confStorage->save();
                             }

                             request.send(HTML_OK, "text/html", m_resources->getAdminHtml());
                         });

        m_server->onPost("/removeSensor",
                         [this, auth](IWebRequest &request, std::string body)
                         {
                             if (!auth(request))
                             {
                                 request.send(HTML_UNAUTH);
                             }

                             else
                             {
                                 try
                                 {
                                     auto sensor = nlohmann::json::parse(body);
                                     IDType sensorId = sensor["identifier"];
                                     m_confStorage->removeSensor(sensorId);
                                 }
                                 catch (...)
                                 {
                                     request.send(HTML_INTERNAL_ERR);
                                 }
                                 request.send(HTML_OK);
                             }
                         });

        m_server->onGet("/logout",
                        [this](IWebRequest &request)
                        {
                            request.send(HTML_UNAUTH, "text/html", m_resources->getAdminHtml());
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

        m_server->onGet("/pico.min.css",
                        [this](IWebRequest &request)
                        {
                            request.send(HTML_OK, "text/css", m_resources->getPicoCss());
                        });

        m_server->onGet("/sensorIDsToNames",
                        [this](IWebRequest &request)
                        {
                            auto sensorsMappingJsonStr = m_confStorage->getSensorsMapping();
                            logger::logDbg("sensorIDsToNames %s", sensorsMappingJsonStr);
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
                logger::logDbg("Client connected");
                if (client.lastId() != 0)
                {
                    logger::logDbg("Client reconnected, last ID: %u\n", client.lastId());
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
