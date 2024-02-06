#pragma once

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESPAsyncWebServer.h>

#include <memory>

#include "ConfStorage.hpp"
#include "EspNow.hpp"
#include "ReadingsStorage.hpp"
#include "WebView.hpp"
#include "WebWifiConfig.hpp"
#include "InfoLed.hpp"

class App
{
    enum class State
    {
        OK,
        FAIL,
        WIFI_CONFIGURATION_NEEDED
    };

    enum class Mode
    {
        SENSOR_HOST,
        WIFI_SETTINGS
    };

    struct WebViewComponentTypes {
        using AsyncWebSvrType = AsyncWebServer;
        using AsyncEventSrcType = AsyncEventSource;
        using AsyncWebSrvReq = AsyncWebServerRequest;
        using AsyncEventSrcClient = AsyncEventSourceClient;
        using AsyncWebParam = AsyncWebParameter;

        enum ReqMethod {
            GET = HTTP_GET,
            POST = HTTP_POST
        };
    };

    using WebViewType = WebView<WebViewComponentTypes>;
    using WebWifiConfigType = WebWifiConfig<ConfStorage, AsyncWebServer>;

public:
    App() = default;
    void init();
    void update();

private:
    State systemInit();
    State initConfig();
    State connectWiFi();
    void wifiSettingsMode();
    void setupWifiButton();
    bool isWifiButtonPressed();

    constexpr static auto infoLed = 23;
    constexpr static auto wifiButton = 14;
    constexpr static auto wifiConfigServerTimeoutMillis = 1000 * 60 * 10; // 10 minutes

    Mode m_mode = Mode::SENSOR_HOST;

    std::unique_ptr<InfoLed> m_infoLed;
    std::shared_ptr<ConfStorage> m_confStorage{};
    std::unique_ptr<WebViewType> m_web{};
    std::unique_ptr<WebWifiConfigType> m_webWifiConfig{};
    WiFiUDP m_ntpUDP{};
    std::shared_ptr<NTPClient> m_timeClient{};
    std::unique_ptr<EspNow> m_espNow{};
    ReadingsStorage m_readingsStorage{};
};
