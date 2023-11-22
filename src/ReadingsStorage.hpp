#pragma once

#include <map>

#include "MacAddr.hpp"
#include "ReadingsCleaner.hpp"

class ReadingsStorage
{
    struct Reading
    {
        float temperature;
        float humidity;
        unsigned long epochTime;
    };

public:
    void addReading(const MacAddr &mac,
                    const std::string &sensorName,
                    float temperature,
                    float humidity,
                    unsigned long epochTime);
    std::map<MacAddr, Reading> &getReadings();

private:
    std::map<MacAddr, Reading> m_readings;
    ReadingsCleaner m_readingsCleaner;

    void saveReading(MacAddr mac, const std::string &sensorName, const Reading &reading);
};
