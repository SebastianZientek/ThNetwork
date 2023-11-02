#include "ReadingsStorage.hpp"

void ReadingsStorage::addReading(const MacAddr &mac,
                                 float temperature,
                                 float humidity,
                                 unsigned long epochTime)
{
    auto fracVal = temperature - static_cast<int>(temperature);  // NOLINT
    int8_t frac = fracVal > 0.5 ? 1 : 0;                         // NOLINT

    Reading reading = {temperature, humidity, epochTime};
    m_readings[mac] = reading;
}
