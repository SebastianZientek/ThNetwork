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
        std::string reading;
    };

    constexpr static uint16_t maxReadingsPerSensor = 220;
    using ReadingsRingBuffer = RingBuffer<Reading, maxReadingsPerSensor>;

public:
    void addReading(const MacAddr &mac,
                    const std::string &sensorName,
                    float temperature,
                    float humidity,
                    unsigned long epochTime);
    std::map<MacAddr, ReadingsRingBuffer> &getReadingBuffers();
    std::string getReadingsAsJsonArr(const MacAddr &macAddr, const std::string &sensorName);
    std::string lastReading(const MacAddr &macAddr, const std::string &sensorName);

private:
    std::map<MacAddr, ReadingsRingBuffer> m_readingBuffers;

    std::string readingToStr(float temperature, float humidity, unsigned long epochTime);
};
