#include "WiFiAdp.hpp"

#include <ESP8266WiFi.h>

void WiFiAdp::setModeSta() { WiFi.mode(WIFI_STA); }

void WiFiAdp::setChannel(uint8_t channel)
{
    wifi_promiscuous_enable(1);
    wifi_set_channel(channel);
    wifi_promiscuous_enable(0);
}

void WiFiAdp::disconnect() { WiFi.disconnect(); }

uint8_t *WiFiAdp::macAddress(uint8_t *mac) { return WiFi.macAddress(mac); }
