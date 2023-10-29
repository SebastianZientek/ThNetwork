#pragma once

#include <map>

#include "MacAddr.hpp"
#include "RingBuffer.hpp"

class ReadingsStorage
{
    struct Reading
    {
        int8_t temperature : 7, frac : 1;  // If point == 1 -> +0.5 to temperature
        int8_t humidity;
    };

public:
    void addReading(MacAddr mac, float temperature, float humidity);
    std::pair<float, float> getReading();

private:
    std::map<MacAddr, Reading> m_readings;
    RingBuffer<Reading, 5> test;
};
