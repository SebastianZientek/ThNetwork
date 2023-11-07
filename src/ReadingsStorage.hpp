#pragma once

#include <map>

#include "MacAddr.hpp"

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
                    const String &sensorName,
                    float temperature,
                    float humidity,
                    unsigned long epochTime);
    std::map<MacAddr, Reading> &getReadings();

private:
    std::map<MacAddr, Reading> m_readings;

    void saveReading(MacAddr mac, const String &sensorName, const Reading &reading);
};
