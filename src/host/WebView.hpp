#pragma once

#include <ESPAsyncWebServer.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>

#include "common/logger.hpp"

extern "C"
{
    extern const char gIndexHtmlData[];
    extern const char gAdminHtmlData[];
    extern const char gMicroChartData[];
    extern const unsigned char gFaviconData[];
    extern const unsigned int gFaviconSize;
}

template <typename ConfStorageType, typename AsyncWebServerType, typename AsyncEventSourceType>
class WebView
{
    using GetSensorDataCb = std::function<std::string(const std::size_t &)>;

public:
    WebView(int port, std::shared_ptr<ConfStorageType> confStorage)
        : m_server(port)
        , m_events("/events")
        , m_confStorage(confStorage)
    {
    }

    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0);

    void startServer(const GetSensorDataCb &getSensorDataCb);
    void stopServer();

private:
    AsyncWebServerType m_server;
    AsyncEventSourceType m_events;
    std::shared_ptr<ConfStorageType> m_confStorage;

    GetSensorDataCb m_getSensorDataCb;
};

template <typename ConfStorageType, typename AsyncWebServerType, typename AsyncEventSourceType>
void WebView<ConfStorageType, AsyncWebServerType, AsyncEventSourceType>::sendEvent(
    const char *message, const char *event, uint32_t identifier, uint32_t reconnect)

{
    logger::logInf("Send event %s, %lu", event, identifier);
    m_events.send(message, event, identifier, reconnect);
}

template <typename ConfStorageType, typename AsyncWebServerType, typename AsyncEventSourceType>
void WebView<ConfStorageType, AsyncWebServerType, AsyncEventSourceType>::startServer(
    const GetSensorDataCb &getSensorDataCb)
{
    m_getSensorDataCb = getSensorDataCb;

    constexpr auto HTML_OK = 200;
    constexpr auto HTML_UNAUTH = 401;

    constexpr auto RECONNECT_TIMEOUT = 10000;

    auto auth = [this](AsyncWebServerRequest *request)
    {
        auto [user, passwd] = m_confStorage->getCredentials();
        return request->authenticate(user.c_str(), passwd.c_str());
    };

    m_server.on("/", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                { request->send_P(HTML_OK, "text/html", gIndexHtmlData); });

    m_server.on("/admin", HTTP_GET,
                [this, auth](AsyncWebServerRequest *request)
                {
                    if (!auth(request))
                    {
                        return request->requestAuthentication();
                    }
                    request->send_P(HTML_OK, "text/html", gAdminHtmlData);
                });

    m_server.on("/setCredentials", HTTP_POST,
                [this](AsyncWebServerRequest *request)
                {
                    auto params = request->params();
                    std::string sensorName{};
                    for (int i = 0; i < params; i++)
                    {
                        AsyncWebParameter *param = request->getParam(i);
                        if (param->name() == "sensor")
                        {
                            sensorName = param->value().c_str();
                            break;
                        }
                    }

                    request->send_P(HTML_OK, "text/html", gAdminHtmlData);
                });

    m_server.on("/logout", HTTP_GET,
                [](AsyncWebServerRequest *request) { request->send(HTML_UNAUTH); });

    m_server.on("/favicon.ico", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                { request->send_P(HTML_OK, "image/png", gFaviconData, gFaviconSize); });

    m_server.on("/microChart.js", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                { request->send_P(HTML_OK, "application/javascript", gMicroChartData); });

    m_server.on("/sensorIDsToNames", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                {
                    logger::logInf("sensorIDsToNames %s",
                                   m_confStorage->getSensorIDsToNamesJsonStr().c_str());
                    request->send_P(HTML_OK, "application/json",
                                    m_confStorage->getSensorIDsToNamesJsonStr().c_str());
                });

    m_server.on("/configuration", HTTP_GET,
                [this, auth](AsyncWebServerRequest *request)
                {
                    if (!auth(request))
                    {
                        request->send(HTML_UNAUTH);
                    }

                    auto config = m_confStorage->getConfigWithoutCredentials();
                    request->send_P(HTML_OK, "application/json", config.dump().c_str());
                });

    m_server.on("/sensorData", HTTP_GET,
                [this](AsyncWebServerRequest *request)
                {
                    logger::logInf("sensorsData");
                    auto params = request->params();
                    std::size_t identifier{};
                    for (int i = 0; i < params; i++)
                    {
                        AsyncWebParameter *param = request->getParam(i);
                        if (param->name() == "identifier")
                        {
                            identifier = std::stoull(param->value().c_str());
                            break;
                        }
                    }

                    request->send_P(HTML_OK, "application/json",
                                    m_getSensorDataCb(identifier).c_str());
                });

    m_events.onConnect(
        [this](AsyncEventSourceClient *client)
        {
            logger::logInf("Client connected");
            if (client->lastId() != 0)
            {
                logger::logInf("Client reconnected, last ID: %u\n", client->lastId());
            }
            client->send("init", nullptr, millis(), RECONNECT_TIMEOUT);
        });
    m_server.addHandler(&m_events);
    m_server.begin();
}

template <typename ConfStorageType, typename AsyncWebServerType, typename AsyncEventSourceType>
void WebView<ConfStorageType, AsyncWebServerType, AsyncEventSourceType>::stopServer()
{
    m_events.close();
    m_server.end();
}
