#pragma once

#include "EspNow.hpp"
#include "WebView.hpp"
#include "boardsettings.hpp"
#include "Config.hpp"
#include "ReadingsStorage.hpp"

class App
{
    enum class Status
    {
        OK,
        FAIL
    };

public:
    void run();

private:
    Status systemInit();
    Status initSD();
    Status readConfig();
    Status saveExampleConfig();
    Status connectWiFi();

    WebView m_web{boardsettings::serverPort};
    EspNow m_espNow;
    Config m_config;
    ReadingsStorage m_readings;
};
