#pragma once

#include <ESPAsyncWebServer.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

class Request
{
public:
    explicit Request(AsyncWebServerRequest *request)
        : m_request(request)
    {
    }

    void send(int code, std::string contentType, const uint8_t *content, size_t len)
    {
        m_request->send_P(code, contentType.c_str(), content, len);
    }

    void send(int code, std::string contentType, const char *content)
    {
        m_request->send_P(code, contentType.c_str(), content);
    }

    void send(int code)
    {
        m_request->send(code);
    }

    bool authenticate(const std::string &user, const std::string &passwd)
    {
        return m_request->authenticate(user.c_str(), passwd.c_str());
    }

    void requestAuthentication()
    {
        m_request->requestAuthentication();
    }

    std::map<std::string, std::string> getParams()
    {
        std::map<std::string, std::string> paramsMap;
        auto paramsNumber = m_request->params();

        for (std::size_t idx = 0; idx < paramsNumber; ++idx)
        {
            AsyncWebParameter *param = m_request->getParam(idx);
            const auto *paramName = param->name().c_str();
            const auto *paramValue = param->value().c_str();
            paramsMap[paramName] = paramValue;
        }

        return paramsMap;
    }

private:
    AsyncWebServerRequest *m_request;
};

class EventSrcClient
{
public:
    explicit EventSrcClient(AsyncEventSourceClient *client)
        : m_client(client)
    {
    }
    uint32_t lastId()
    {
        return m_client->lastId();
    }

    void send(const char *message,
              const char *event = NULL,
              uint32_t id = 0,
              uint32_t reconnect = 0)
    {
        m_client->send(message, event, id, reconnect);
    }

private:
    AsyncEventSourceClient *m_client;
};

class WebServer
{
public:
    void start()
    {
        m_server.begin();
    }

    void stop()
    {
        if (m_events)
        {
            m_events->close();
        }
        m_server.end();
    }

    void onGet(std::string url, std::function<void(Request)> clbk)
    {
        m_server.on(url.c_str(), HTTP_GET,
                    [clbk](AsyncWebServerRequest *request) { clbk(Request(request)); });
    }

    void onPost(std::string url, std::function<void(Request)> clbk)
    {
        m_server.on(url.c_str(), HTTP_POST,
                    [clbk](AsyncWebServerRequest *request) { clbk(Request(request)); });
    }

    void setupEventsSource(const std::string &src,
                           std::function<void(const EventSrcClient &)> onConnectClbk)
    {
        m_events = std::make_unique<AsyncEventSource>(src.c_str());
        m_events->onConnect([onConnectClbk](AsyncEventSourceClient *client)
                            { onConnectClbk(EventSrcClient(client)); });
        m_server.addHandler(m_events.get());
    }

    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0)

    {
        m_events->send(message, event, identifier, reconnect);
    }

private:
    AsyncWebServer m_server{80};
    std::unique_ptr<AsyncEventSource> m_events;
};
