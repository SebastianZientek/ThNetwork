#include "ReadingsStorage.hpp"

#include "RaiiFile.hpp"
#include "logger.hpp"
#include "utils.hpp"

void ReadingsStorage::addReading(const MacAddr &mac,
                                 float temperature,
                                 float humidity,
                                 unsigned long epochTime)
{
    auto fracVal = temperature - static_cast<int>(temperature);  // NOLINT
    int8_t frac = fracVal > 0.5 ? 1 : 0;                         // NOLINT

    Reading reading = {temperature, humidity, epochTime};
    m_readings[mac] = reading;

    saveReading(mac, reading, epochTime);
}

std::map<MacAddr, ReadingsStorage::Reading> &ReadingsStorage::getReadings() { return m_readings; }

void ReadingsStorage::saveReading(MacAddr mac, const Reading &reading, unsigned long epochTime)
{
    auto sensorId = mac.str();
    sensorId.replace(':', '_');

    // String path = "/readings/" + sensorId + ;

    logger::logInf(utils::epochToFormattedDate(epochTime));
    // RaiiFile test("/test/file.dat", FILE_APPEND, true);
    // test.println("blabla");
}
