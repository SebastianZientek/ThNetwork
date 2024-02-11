#include "ReadingsStorage.hpp"

#include <cstdio>
#include <nlohmann/json.hpp>
#include <string>

#include "RaiiFile.hpp"
#include "common/logger.hpp"

void ReadingsStorage::addReading(IDType identifier,
                                 float temperature,
                                 float humidity,
                                 unsigned long epochTime)
{
    m_readingBuffers[identifier].put({temperature, humidity, epochTime});
}

std::string ReadingsStorage::getReadingsAsJsonStr(IDType identifier)
{
    ReadingsRingBuffer &readingsBuffer = m_readingBuffers[identifier];
    auto jsonData = nlohmann::json::array();

    for (const auto &reading : readingsBuffer)
    {
        jsonData.push_back({reading.epochTime, reading.temperature, reading.humidity});
    }

    auto json = nlohmann::json();
    json["values"] = jsonData;
    json["identifier"] = identifier;

    return json.dump();
}

std::string ReadingsStorage::getLastReadingAsJsonStr(IDType identifier)
{
    ReadingsRingBuffer &readingsBuffer = m_readingBuffers[identifier];
    const auto &lastReading = readingsBuffer.getLast();

    auto json = nlohmann::json();
    if (readingsBuffer.begin() != readingsBuffer.end())
    {
        auto jsonData = nlohmann::json::array(
            {{lastReading.epochTime, lastReading.temperature, lastReading.humidity}});
        json["values"] = jsonData;
    }
    else
    {
        json["values"] = nlohmann::json::array();
    }
    json["identifier"] = identifier;

    return json.dump();
}
