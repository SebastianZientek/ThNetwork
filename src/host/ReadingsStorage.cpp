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
    m_readingBuffers[mac].put(reading);
}

std::map<MacAddr, ReadingsStorage::ReadingsRingBuffer> &ReadingsStorage::getReadingBuffers()
{
    return m_readingBuffers;
}
