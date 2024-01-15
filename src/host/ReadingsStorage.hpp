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
    void addReading(IDType identifier,
                    const std::string &sensorName,
                    float temperature,
                    float humidity,
                    unsigned long epochTime);
    std::map<IDType, ReadingsRingBuffer> &getReadingBuffers();
    std::string getReadingsAsJsonArr(IDType identifier, const std::string &sensorName);
    std::string lastReading(IDType identifier, const std::string &sensorName);

private:
    std::map<IDType, ReadingsRingBuffer> m_readingBuffers;
};
