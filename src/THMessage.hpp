#pragma once

#include <cstdint>

enum MessageType : uint8_t
{
    PAIR_REQ = 0,
    PAIR_OK = 1,
    TH_DATA = 2,
};

#pragma pack(1)
struct THMessage
{
    char signature[4];  // NOLINT

    MessageType type;
    uint8_t mac[6];  // NOLINT
    uint8_t channel;

    uint8_t updatePeriodMins;

    float temperature;
    float humidity;
};
#pragma pack(0)
