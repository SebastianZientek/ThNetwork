#pragma once

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <memory>

#include "Config.hpp"
#include "EspNow.hpp"
#include "ReadingsStorage.hpp"
#include "WebView.hpp"
#include "boardsettings.hpp"

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

    WebView m_web{boardsettings::serverPort};
    WiFiUDP m_ntpUDP{};
    std::shared_ptr<NTPClient> m_timeClient{std::make_shared<NTPClient>(m_ntpUDP)};
    EspNow m_espNow{m_timeClient};
    Config m_config{};
    ReadingsStorage m_readings{};
};
