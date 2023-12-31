#pragma once

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <memory>

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
    void sendEvent(float temp, float hum, MacAddr mac, unsigned long epochTime);

    std::unique_ptr<WebView> m_web{};
    WiFiUDP m_ntpUDP{};
    std::shared_ptr<NTPClient> m_timeClient{};
    std::unique_ptr<EspNow> m_espNow{};
    Config m_config{};
    ReadingsStorage m_readings{};
};
