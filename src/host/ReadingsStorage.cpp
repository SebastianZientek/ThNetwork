#include "ReadingsStorage.hpp"

#include "RaiiFile.hpp"
#include "common/logger.hpp"
#include "utils.hpp"

void ReadingsStorage::addReading(const MacAddr &mac,
                                 const std::string &sensorName,
                                 float temperature,
                                 float humidity,
                                 unsigned long epochTime)
{
    Reading reading = {temperature, humidity, epochTime};
    m_readings[mac] = reading;

    saveReading(mac, sensorName, reading);
}

std::map<MacAddr, ReadingsStorage::Reading> &ReadingsStorage::getReadings()
{
    return m_readings;
}

void ReadingsStorage::saveReading(MacAddr mac,
                                  const std::string &sensorName,
                                  const Reading &reading)
{
    std::string path = "/readings/" + utils::epochToFormattedDate(reading.epochTime)  // NOLINT
                       + std::string("/")                                             // NOLINT
                       + sensorName + std::string(".json");                           // NOLINT

    RaiiFile dataFile(path, FILE_APPEND, true);

    std::string macStr = "\"" + mac.str() + "\"";  // NOLINT
    dataFile->print("[");
    dataFile->print(macStr.c_str());
    dataFile->print(",");
    dataFile->print(reading.epochTime);
    dataFile->print(",");
    dataFile->print(reading.temperature);
    dataFile->print(",");
    dataFile->print(reading.humidity);
    dataFile->println("],");

    m_readingsCleaner.cleanIfNeeded();
}
