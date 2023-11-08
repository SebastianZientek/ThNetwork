#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include <MacAddr.hpp>

class WebView
{
    using NewClientCb = std::function<void()>;


public:
    WebView(int port);

    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0);

    void startServer(const NewClientCb &newClientCb = [] {});

private:
    String m_htmlData;
    AsyncWebServer m_server;
    AsyncEventSource m_events;
    NewClientCb m_newClientCb;
};
