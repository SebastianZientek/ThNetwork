#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include <string_view>

class WebView
{
    using GetSensorNamesCb = std::function<std::string()>;
    using GetSensorDataCb = std::function<std::string(const std::string &)>;

public:
    WebView(int port);

    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0);

    void startServer(
        const GetSensorNamesCb &getSensorNamesCb = [] { return ""; },
        const GetSensorDataCb &getSensorDataCb = [](const std::string &) { return ""; });

private:
    std::string m_htmlData;
    AsyncWebServer m_server;
    AsyncEventSource m_events;
    GetSensorNamesCb m_getSensorNamesCb;
    GetSensorDataCb m_getSensorDataCb;
};
