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
    Reading reading = {temperature, humidity, epochTime};
    m_readings[mac] = reading;

    saveReading(mac, sensorName, reading);
}

std::map<MacAddr, ReadingsStorage::Reading> &ReadingsStorage::getReadings() { return m_readings; }

void ReadingsStorage::saveReading(MacAddr mac, const String &sensorName, const Reading &reading)
{
    String path = "/readings/" + utils::epochToFormattedDate(reading.epochTime) + String("/")
                  + sensorName + String(".json");
    RaiiFile dataFile(path, FILE_APPEND, true);
    dataFile->print("[");
    dataFile->print("\"" + mac.str() + "\"");
    dataFile->print(",");
    dataFile->print(reading.epochTime);
    dataFile->print(",");
    dataFile->print(reading.temperature);
    dataFile->print(",");
    dataFile->print(reading.humidity);
    dataFile->println("],");

    m_readingsCleaner.cleanIfNeeded();
}
