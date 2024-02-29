#include "Wifi8266Adp.hpp"

#include <ESP8266WiFi.h>

void Wifi8266Adp::setModeSta() const
{
    WiFi.mode(WIFI_STA);
}

void Wifi8266Adp::setChannel(uint8_t channel) const
{
    wifi_promiscuous_enable(1);
    wifi_set_channel(channel);
    wifi_promiscuous_enable(0);
}

void Wifi8266Adp::disconnect() const
{
    WiFi.disconnect();
}

uint8_t *Wifi8266Adp::macAddress(uint8_t *mac) const
{
    return WiFi.macAddress(mac);
}

[[nodiscard]] std::string Wifi8266Adp::macAddress() const
{
    return WiFi.macAddress().c_str();
}
