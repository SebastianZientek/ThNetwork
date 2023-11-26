#pragma once

#include <cstdint>
#include <string>

struct WiFiAdp
{
    static void setModeSta();
    static void setChannel(uint8_t channel);
    static void disconnect();
    static uint8_t *macAddress(uint8_t *mac);
    static std::string macAddress();
};
