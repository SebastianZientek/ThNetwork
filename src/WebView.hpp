#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "RaiiFile.hpp"

class WebView
{
public:
    WebView(int port);

    void load(const String &path);
    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0);
    void startServer();

private:
    String m_pageData;
    AsyncWebServer m_server;
    AsyncEventSource m_events;
};
