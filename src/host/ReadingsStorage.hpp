#pragma once

#include <map>

#include "RingBuffer.hpp"
#include "common/types.hpp"

class ReadingsStorage
{
    struct Reading
    {
        float temperature;
        float humidity;
        unsigned long epochTime;
    };

    constexpr static uint16_t maxReadingsPerSensor = 220;
    using ReadingsRingBuffer = RingBuffer<Reading, maxReadingsPerSensor>;

public:
    void addReading(IDType identifier, float temperature, float humidity, unsigned long epochTime);
    std::string getReadingsAsJsonArrStr(IDType identifier);
    std::string getLastReadingAsJsonStr(IDType identifier);
    std::string lastReading(IDType identifier, const std::string &sensorName);

private:
    std::map<IDType, ReadingsRingBuffer> m_readingBuffers;
};
