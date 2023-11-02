#pragma once

#include <map>

#include "MacAddr.hpp"
#include "RingBuffer.hpp"

class ReadingsStorage
{
    struct Reading
    {
        float temperature;
        float humidity;
        unsigned long epochTime;
    };

public:
    void addReading(const MacAddr &mac, float temperature, float humidity, unsigned long epochTime);
    std::map<MacAddr, Reading> &getReadings() { return m_readings; }

private:
    std::map<MacAddr, Reading> m_readings;
    // RingBuffer<Reading, 5> test;
};
