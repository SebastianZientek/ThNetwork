#pragma once

#include <ESPAsyncWebServer.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "EventSrcClient.hpp"
#include "IWebServer.hpp"
#include "WebRequest.hpp"

class WebServer : public IWebServer
{
public:
    WebServer(uint16_t port);

    void start() override;
    void stop() override;
    void onGet(std::string url, WebRequestClbk clbk) override;
    void onPost(std::string url, WebRequestClbk clbk) override;
    void onPost(std::string url, WebRequestWithBodyClbk clbk) override;
    void setupEventsSource(const std::string &src, EventClbk onConnectClbk) override;
    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0) override;

private:
    AsyncWebServer m_server;
    std::unique_ptr<AsyncEventSource> m_events;
};
