#pragma once

#include <ESPAsyncWebServer.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>

#include "ConfStorage.hpp"
#include "common/logger.hpp"

extern "C"
{
    extern const char gIndexHtmlData[];
    extern const char gAdminHtmlData[];
    extern const char gMicroChartData[];
    extern const unsigned char gFaviconData[];
    extern const unsigned int gFaviconSize;
}

class WebView
{
    using GetSensorNamesCb = std::function<std::string()>;
    using GetSensorDataCb = std::function<std::string(const std::string &)>;

public:
    WebView(int port, std::shared_ptr<ConfStorage> confStorage)
        : m_server(port)
        , m_events("/events")
        , m_confStorage(confStorage)
    {
    }

    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0)

    {
        logger::logInf("Send event %s, %lu", event, identifier);
        m_events.send(message, event, identifier, reconnect);
    }

    void startServer(const GetSensorNamesCb &getSensorNamesCb,
                     const GetSensorDataCb &getSensorDataCb)
    {
        m_getSensorNamesCb = getSensorNamesCb;
        m_getSensorDataCb = getSensorDataCb;

        auto auth = [this](AsyncWebServerRequest *request)
        {
            auto [user, passwd] = m_confStorage->getCredentials();
            return request->authenticate(user.c_str(), passwd.c_str());
        };

        m_server.on("/", HTTP_GET,
                    [this](AsyncWebServerRequest *request)
                    { request->send_P(200, "text/html", gIndexHtmlData); });

        m_server.on("/admin", HTTP_GET,
                    [this, auth](AsyncWebServerRequest *request)
                    {
                        if (!auth(request))
                        {
                            return request->requestAuthentication();
                        }
                        request->send_P(200, "text/html", gAdminHtmlData);
                    });

        m_server.on("/setCredentials", HTTP_POST,
                    [this](AsyncWebServerRequest *request)
                    {
                        int params = request->params();
                        std::string sensorName{};
                        for (int i = 0; i < params; i++)
                        {
                            AsyncWebParameter *p = request->getParam(i);
                            if (p->name() == "sensor")
                            {
                                sensorName = p->value().c_str();
                                break;
                            }
                        }

                        request->send_P(200, "text/html", gAdminHtmlData);
                    });

        m_server.on("/logout", HTTP_GET,
                    [](AsyncWebServerRequest *request) { request->send(401); });

        m_server.on("/favicon.ico", HTTP_GET,
                    [this](AsyncWebServerRequest *request)
                    { request->send_P(200, "image/png", gFaviconData, gFaviconSize); });

        m_server.on("/microChart.js", HTTP_GET,
                    [this](AsyncWebServerRequest *request)
                    { request->send_P(200, "application/javascript", gMicroChartData); });

        m_server.on("/sensorNames", HTTP_GET,
                    [this](AsyncWebServerRequest *request)
                    { request->send_P(200, "application/json", m_getSensorNamesCb().c_str()); });

        m_server.on("/configuration", HTTP_GET,
                    [this, auth](AsyncWebServerRequest *request)
                    {
                        if (!auth(request))
                        {
                            request->send(401);
                        }

                        auto config = m_confStorage->getConfigWithoutCredentials();

                        request->send_P(200, "application/json", config.dump().c_str());
                    });

        m_server.on("/sensorData", HTTP_GET,
                    [this](AsyncWebServerRequest *request)
                    {
                        int params = request->params();
                        std::string sensorName{};
                        for (int i = 0; i < params; i++)
                        {
                            AsyncWebParameter *p = request->getParam(i);
                            if (p->name() == "sensor")
                            {
                                sensorName = p->value().c_str();
                                break;
                            }
                        }

                        request->send_P(200, "application/json",
                                        m_getSensorDataCb(sensorName).c_str());
                    });

        m_events.onConnect(
            [this](AsyncEventSourceClient *client)
            {
                logger::logInf("Client connected");
                if (client->lastId())
                {
                    logger::logInf("Client reconnected, last ID: %u\n", client->lastId());
                }
                client->send("init", NULL, millis(), 10000);
            });
        m_server.addHandler(&m_events);
        m_server.begin();
    }

private:
    AsyncWebServer m_server;
    AsyncEventSource m_events;
    std::shared_ptr<ConfStorage> m_confStorage;

    GetSensorNamesCb m_getSensorNamesCb;
    GetSensorDataCb m_getSensorDataCb;
};
