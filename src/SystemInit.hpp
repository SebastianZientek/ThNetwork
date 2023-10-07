#pragma once

#include "Config.hpp"

class SystemInit
{
    enum class Status{
        OK,
        FAIL
    };
public:
    void init();

private:
    Config m_config;

    Status initSerial();
    Status initSD();
    Status readConfig();
    Status connectWiFi();
};