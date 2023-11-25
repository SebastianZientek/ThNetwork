#pragma once

#include <cstdint>

struct WiFiAdp
{
    static void setModeSta();
    static void setChannel(uint8_t channel);
    static void disconnect();
    static uint8_t * macAddress(uint8_t* mac);
};
