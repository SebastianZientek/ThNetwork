#pragma once

#include <map>
#include <string_view>

#include "RingBuffer.hpp"
#include "common/types.hpp"

class ReadingsStorage
{
    using Reading = std::string;
    constexpr static uint16_t maxReadingsPerSensor = 220;
    using ReadingsRingBuffer = RingBuffer<Reading, maxReadingsPerSensor>;

public:
    void addReading(IDType ID,
                    const std::string &sensorName,
                    float temperature,
                    float humidity,
                    unsigned long epochTime);
    std::map<IDType, ReadingsRingBuffer> &getReadingBuffers();
    std::string getReadingsAsJsonArr(IDType ID, const std::string &sensorName);
    std::string lastReading(IDType ID, const std::string &sensorName);

private:
    std::map<IDType, ReadingsRingBuffer> m_readingBuffers;

    std::string readingToStr(float temperature, float humidity, unsigned long epochTime);
};
