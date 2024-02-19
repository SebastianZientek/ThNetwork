#pragma once

#include <ESP8266WiFi.h>

#include "IWifi8266Adp.hpp"

class Wifi8266Adp : public IWifi8266Adp
{
public:
    void setModeSta() override
    {
        WiFi.mode(WIFI_STA);
    }
    void setChannel(uint8_t channel) override
    {
        wifi_promiscuous_enable(1);
        wifi_set_channel(channel);
        wifi_promiscuous_enable(0);
    }
    void disconnect() override
    {
        WiFi.disconnect();
    }
    uint8_t *macAddress(uint8_t *mac) override
    {
        return WiFi.macAddress(mac);
    }
    std::string macAddress() override
    {
        return WiFi.macAddress().c_str();
    }
};
