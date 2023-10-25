#pragma once

#include "EspNow.hpp"
#include "WebView.hpp"
#include "boardsettings.hpp"
#include "Config.hpp"

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
    Status connectWiFi();

    WebView m_web{boardsettings::serverPort};
    EspNow m_espNow;
    Config m_config;
};
