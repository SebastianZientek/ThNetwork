#include "ReadingsStorage.hpp"

void ReadingsStorage::addReading(MacAddr mac, float temperature, float humidity)
{
    auto fracVal = temperature - static_cast<int>(temperature);  // NOLINT
    int8_t frac = fracVal > 0.5 ? 1 : 0;                         // NOLINT

    Reading reading = {.temperature = static_cast<int8_t>(temperature),
                       .frac = frac,
                       .humidity = static_cast<int8_t>(humidity)};
    // m_readings[mac] = reading;
}
