#pragma once

#include <map>

#include "RingBuffer.hpp"
#include "common/types.hpp"

class ReadingsStorage
{
public:
    void addReading(IDType identifier, float temperature, float humidity, unsigned long epochTime);
    std::string getReadingsAsJsonStr(IDType identifier);
    std::string getLastReadingAsJsonStr(IDType identifier);

private:
    struct Reading
    {
        float temperature;
        float humidity;
        unsigned long epochTime;
    };

    constexpr static uint16_t maxReadingsPerSensor = 220;
    using ReadingsRingBuffer = RingBuffer<Reading, maxReadingsPerSensor>;

    std::map<IDType, ReadingsRingBuffer> m_readingBuffers;
};
