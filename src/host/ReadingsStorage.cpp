#include "ReadingsStorage.hpp"

#include <cstdio>
#include <string>

#include "RaiiFile.hpp"
#include "common/logger.hpp"
#include "utils.hpp"

void ReadingsStorage::addReading(IDType ID,
                                 const std::string &sensorName,
                                 float temperature,
                                 float humidity,
                                 unsigned long epochTime)
{
    std::string jsonArr = readingToStr(temperature, humidity, epochTime);
    m_readingBuffers[ID].put(jsonArr);
}

std::map<IDType, ReadingsStorage::ReadingsRingBuffer> &ReadingsStorage::getReadingBuffers()
{
    return m_readingBuffers;
}

std::string ReadingsStorage::getReadingsAsJsonArr(IDType ID, const std::string &sensorName)
{
    ReadingsRingBuffer &readingsBuffer = m_readingBuffers[ID];

    std::string jsonStr = R"({"data":[)";
    bool first = true;

    for (const auto &reading : readingsBuffer)
    {
        if (first)
        {
            jsonStr += '[' + reading + ']';
            first = false;
        }
        else
        {
            jsonStr += ",[" + reading + "]";
        }
    }

    jsonStr += R"(], "sensor":")" + sensorName + "\"}";

    return jsonStr;
}

std::string ReadingsStorage::lastReading(IDType ID, const std::string &sensorName)
{
    ReadingsRingBuffer &readingsBuffer = m_readingBuffers[ID];
    const auto &readingStr = readingsBuffer.getLast();

    return R"([")" + sensorName + R"(",)" + readingStr + "]";
}

std::string ReadingsStorage::readingToStr(float temperature,
                                          float humidity,
                                          unsigned long epochTime)
{
    std::array<char, 5> numberBuffer{};

    std::string jsonArr = std::to_string(epochTime) + ',';
    sprintf(numberBuffer.data(), "%0.1f,", temperature);
    jsonArr += numberBuffer.data();
    sprintf(numberBuffer.data(), "%0.1f", humidity);
    jsonArr += numberBuffer.data();

    return jsonArr;
}
