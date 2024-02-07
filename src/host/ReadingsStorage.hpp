#pragma once

#include <map>
#include <string_view>

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
    std::string getReadingsAsJsonArr(IDType identifier);
    std::string getLastReadingAsJson(IDType identifier);
    std::string lastReading(IDType identifier, const std::string &sensorName);

private:
    std::map<IDType, ReadingsRingBuffer> m_readingBuffers;
};
