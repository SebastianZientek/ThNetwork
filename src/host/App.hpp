#pragma once

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <memory>

#include "ConfStorage.hpp"
#include "Config.hpp"
#include "EspNow.hpp"
#include "ReadingsStorage.hpp"
#include "WebView.hpp"

class App
{
    enum class Status
    {
        OK,
        FAIL
    };

    using WebViewType = WebView<ConfStorage, AsyncWebServer, AsyncEventSource>;

public:
    App() = default;
    void init();
    void update();

private:
    Status systemInit();
    Status initSD();
    Status readConfig();
    Status saveExampleConfig();
    Status connectWiFi();

    std::shared_ptr<ConfStorage> m_confStorage{};
    std::unique_ptr<WebViewType> m_web{};
    WiFiUDP m_ntpUDP{};
    std::shared_ptr<NTPClient> m_timeClient{};
    std::unique_ptr<EspNow> m_espNow{};
    Config m_config{};
    ReadingsStorage m_readingsStorage{};
};
