#include "ReadingsStorage.hpp"

#include <cstdio>
#include <nlohmann/json.hpp>
#include <string>

#include "RaiiFile.hpp"
#include "common/logger.hpp"
#include "utils.hpp"

void ReadingsStorage::addReading(IDType identifier,
                                 const std::string &sensorName,
                                 float temperature,
                                 float humidity,
                                 unsigned long epochTime)
{
    m_readingBuffers[identifier].put({sensorName, temperature, humidity, epochTime});
}

std::map<IDType, ReadingsStorage::ReadingsRingBuffer> &ReadingsStorage::getReadingBuffers()
{
    return m_readingBuffers;
}

std::string ReadingsStorage::getReadingsAsJsonArr(IDType identifier, const std::string &sensorName)
{
    ReadingsRingBuffer &readingsBuffer = m_readingBuffers[identifier];
    auto jsonData = nlohmann::json::array();

    for (const auto &reading : readingsBuffer)
    {
        jsonData.push_back({reading.epochTime, reading.temperature, reading.humidity});
    }

    auto json = nlohmann::json();
    json["data"] = jsonData;
    json["id"] = identifier;
    json["sensor"] = sensorName;

    return json.dump();
}

std::string ReadingsStorage::lastReading(IDType identifier, const std::string &sensorName)
{
    ReadingsRingBuffer &readingsBuffer = m_readingBuffers[identifier];
    const auto &reading = readingsBuffer.getLast();

    auto json
        = nlohmann::json({sensorName, reading.epochTime, reading.temperature, reading.humidity});

    return json.dump();
}
