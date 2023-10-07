#pragma once 

#include <cstdint>

enum MessageType: uint8_t {
    PAIR_REQ = 0,
    PAIR_OK = 1,
    PAIR_NOK = 2,
    TH_DATA = 3,
};

#pragma pack (1)
struct THMessage{
    char hello[8];
    MessageType type;
    uint8_t mac[6];
    uint8_t channel;
    float temperature;
    float humidity;
};
#pragma pack (0)