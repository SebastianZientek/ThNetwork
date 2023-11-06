#include "ReadingsStorage.hpp"

#include "RaiiFile.hpp"
#include "logger.hpp"
#include "utils.hpp"

void ReadingsStorage::addReading(const MacAddr &mac,
                                 const String &sensorName,
                                 float temperature,
                                 float humidity,
                                 unsigned long epochTime)
{
    auto fracVal = temperature - static_cast<int>(temperature);  // NOLINT
    int8_t frac = fracVal > 0.5 ? 1 : 0;                         // NOLINT

    Reading reading = {temperature, humidity, epochTime};
    m_readings[mac] = reading;

    saveReading(mac, reading);
}

std::map<MacAddr, ReadingsStorage::Reading> &ReadingsStorage::getReadings() { return m_readings; }

void ReadingsStorage::saveReading(MacAddr mac, const Reading &reading)
{
    auto sensorId = mac.str();
    sensorId.replace(':', '_');

    String path = "/readings/" + utils::epochToFormattedDate(reading.epochTime) + String("/")
                  + sensorId + String(".json");

    logger::logInf(utils::epochToFormattedDate(reading.epochTime));
    RaiiFile dataFile(path, FILE_APPEND, true);

    dataFile->print("[");
    dataFile->print(mac);
    dataFile->print(",");
    dataFile->print(reading.epochTime);
    dataFile->print(",");
    dataFile->print(reading.temperature);
    dataFile->print(",");
    dataFile->print(reading.humidity);
    dataFile->println("],");
}
