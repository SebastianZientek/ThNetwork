#pragma once

#include <map>

#include "RingBuffer.hpp"
#include "common/MacAddr.hpp"

class ReadingsStorage
{
    struct Reading
    {
        float temperature;
        float humidity;
        unsigned long epochTime;
    };

    constexpr static uint16_t maxReadingsPerSensor = 800;
    using ReadingsRingBuffer = RingBuffer<Reading, maxReadingsPerSensor>;

public:
    void addReading(const MacAddr &mac,
                    const std::string &sensorName,
                    float temperature,
                    float humidity,
                    unsigned long epochTime);
    std::map<MacAddr, ReadingsRingBuffer> &getReadingBuffers();

private:
    std::map<MacAddr, ReadingsRingBuffer> m_readingBuffers;
};
