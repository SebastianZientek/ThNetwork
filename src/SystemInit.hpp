#pragma once

#include "Config.hpp"

class SystemInit
{
    enum class Status
    {
        OK,
        FAIL
    };

public:
    void init();
    Config getConfig();

private:
    Config m_config;

    Status initLogger();
    Status initSD();
    Status readConfig();
    Status connectWiFi();

    constexpr static auto wifiConnectDelay = 1000;
    constexpr static auto initDelay = 500;
};