#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "RaiiFile.hpp"
#include <MacAddr.hpp>

class WebView
{
    using NewClientCb = std::function<void()>;
public:
    WebView(int port);

    void load(const String &path);
    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0);
    void startServer(NewClientCb newClientCb = []{});

private:
    String m_pageData;
    AsyncWebServer m_server;
    AsyncEventSource m_events;
};
