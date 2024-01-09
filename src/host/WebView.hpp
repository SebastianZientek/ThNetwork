#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>

#include "ConfStorage.hpp"

class WebView
{
    using GetSensorNamesCb = std::function<std::string()>;
    using GetSensorDataCb = std::function<std::string(const std::string &)>;

public:
    WebView(int port, std::shared_ptr<ConfStorage> confStorage);

    void sendEvent(const char *message,
                   const char *event = nullptr,
                   uint32_t identifier = 0,
                   uint32_t reconnect = 0);

    void startServer(const GetSensorNamesCb &getSensorNamesCb,
                     const GetSensorDataCb &getSensorDataCb);

private:
    AsyncWebServer m_server;
    AsyncEventSource m_events;
    std::shared_ptr<ConfStorage> m_confStorage;

    GetSensorNamesCb m_getSensorNamesCb;
    GetSensorDataCb m_getSensorDataCb;
};
