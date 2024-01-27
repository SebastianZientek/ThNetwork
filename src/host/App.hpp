#pragma once

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <memory>

#include "ConfStorage.hpp"
#include "Config.hpp"
#include "EspNow.hpp"
#include "ReadingsStorage.hpp"
#include "WebView.hpp"
#include "WebWifiConfig.hpp"

class App
{
    enum class Status
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

    using WebViewType = WebView<ConfStorage, AsyncWebServer, AsyncEventSource>;
    using WebWifiConfigType = WebWifiConfig<ConfStorage, AsyncWebServer>;

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
    void wifiSettingsMode();
    void setupWifiButton();
    bool isWifiButtonPressed();

    Mode m_mode = Mode::SENSOR_HOST;
    std::shared_ptr<ConfStorage> m_confStorage{};
    std::unique_ptr<WebViewType> m_web{};
    std::unique_ptr<WebWifiConfigType> m_webWifiConfig{};
    WiFiUDP m_ntpUDP{};
    std::shared_ptr<NTPClient> m_timeClient{};
    std::unique_ptr<EspNow> m_espNow{};
    Config m_config{};
    ReadingsStorage m_readingsStorage{};
};
