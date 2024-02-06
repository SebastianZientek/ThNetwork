#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>

#include "IConfStorage.hpp"
#include "Resources.hpp"
#include "common/logger.hpp"

template <typename ComponentTypes>
class WebView
{
    using AsyncWebSvrType = typename ComponentTypes::AsyncWebSvrType;
    using AsyncWebSrvReq = typename ComponentTypes::AsyncWebSrvReq;
    using AsyncEventSrcType = typename ComponentTypes::AsyncEventSrcType;
    using AsyncEventSrcClient = typename ComponentTypes::AsyncEventSrcClient;
    using AsyncWebParam = typename ComponentTypes::AsyncWebParam;

    using GetSensorDataCb = std::function<std::string(const std::size_t &)>;

public:
    WebView(std::shared_ptr<IConfStorage> confStorage)
        : m_server(confStorage->getServerPort())
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

    void startServer(const GetSensorDataCb &getSensorDataCb)
    {
        m_getSensorDataCb = getSensorDataCb;

        constexpr auto HTML_OK = 200;
        constexpr auto HTML_UNAUTH = 401;
        constexpr auto RECONNECT_TIMEOUT = 10000;

        auto auth = [this](AsyncWebSrvReq *request)
        {
            auto credentials = m_confStorage->getAdminCredentials();
            if (!credentials.has_value())
            {
                return false;
            }

            auto [user, passwd] = credentials.value();
            return request->authenticate(user.c_str(), passwd.c_str());
        };

        m_server.on("/", ComponentTypes::ReqMethod::GET,
                    [this](AsyncWebSrvReq *request)
                    { request->send_P(HTML_OK, "text/html", gIndexHtmlData); });

        m_server.on("/admin", ComponentTypes::ReqMethod::GET,
                    [this, auth](AsyncWebSrvReq *request)
                    {
                        if (!auth(request))
                        {
                            return request->requestAuthentication();
                        }
                        request->send_P(HTML_OK, "text/html", gAdminHtmlData);
                    });

        m_server.on("/setCredentials", ComponentTypes::ReqMethod::POST,
                    [this](AsyncWebSrvReq *request)
                    {
                        auto params = request->params();
                        std::string sensorName{};
                        for (int i = 0; i < params; i++)
                        {
                            AsyncWebParam *param = request->getParam(i);
                            if (param->name() == "sensor")
                            {
                                sensorName = param->value().c_str();
                                break;
                            }
                        }

                        request->send_P(HTML_OK, "text/html", gAdminHtmlData);
                    });

        m_server.on("/logout", ComponentTypes::ReqMethod::GET,
                    [](AsyncWebSrvReq *request) { request->send(HTML_UNAUTH); });

        m_server.on("/favicon.ico", ComponentTypes::ReqMethod::GET,
                    [this](AsyncWebSrvReq *request)
                    { request->send_P(HTML_OK, "image/png", gFaviconData, gFaviconSize); });

        m_server.on("/microChart.js", ComponentTypes::ReqMethod::GET,
                    [this](AsyncWebSrvReq *request)
                    { request->send_P(HTML_OK, "application/javascript", gMicroChartData); });

        m_server.on("/sensorIDsToNames", ComponentTypes::ReqMethod::GET,
                    [this](AsyncWebSrvReq *request)
                    {
                        auto sensorsMappingJsonStr = m_confStorage->getSensorsMapping().dump();
                        logger::logInf("sensorIDsToNames %s", sensorsMappingJsonStr.c_str());
                        request->send_P(HTML_OK, "application/json", sensorsMappingJsonStr.c_str());
                    });

        m_server.on("/configuration", ComponentTypes::ReqMethod::GET,
                    [this, auth](AsyncWebSrvReq *request)
                    {
                        if (!auth(request))
                        {
                            request->send(HTML_UNAUTH);
                        }

                        auto config = m_confStorage->getConfigWithoutCredentials();
                        request->send_P(HTML_OK, "application/json", config.dump().c_str());
                    });

        m_server.on("/sensorData", ComponentTypes::ReqMethod::GET,
                    [this](AsyncWebSrvReq *request)
                    {
                        logger::logInf("sensorsData");
                        auto params = request->params();
                        std::size_t identifier{};
                        for (int i = 0; i < params; i++)
                        {
                            AsyncWebParam *param = request->getParam(i);
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
            [this](AsyncEventSrcClient *client)
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

    void stopServer()
    {
        m_events.close();
        m_server.end();
    }

private:
    AsyncWebSvrType m_server;
    AsyncEventSrcType m_events;
    std::shared_ptr<IConfStorage> m_confStorage;

    GetSensorDataCb m_getSensorDataCb;
};
