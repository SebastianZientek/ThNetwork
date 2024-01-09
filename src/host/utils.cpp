#include "utils.hpp"

#include <ArduinoJson.h>

#include <ctime>
#include <Crypto.h>
#include "ArduinoJson/Document/StaticJsonDocument.hpp"
#include "ArduinoJson/Json/JsonSerializer.hpp"

namespace
{
std::string floatToOneDecimalStr(float val)
{
    constexpr auto initialBufferSize = 10;
    std::array<char, initialBufferSize> buf{};
    std::sprintf(buf.data(), "%.1f", val);  // NOLINT
    return buf.data();
}
}  // namespace

namespace utils
{
constexpr auto readingsJsonSize = 120;

std::string readingsToJsonString(
    float temp, float hum, MacAddr mac, const std::string &sensorName, unsigned long epochTime)
{
    constexpr auto initialBufferSize = 5;

    StaticJsonDocument<readingsJsonSize> readings{};  // NOLINT
    readings["epochTime"] = epochTime;
    readings["temperature"] = floatToOneDecimalStr(temp);
    readings["humidity"] = floatToOneDecimalStr(hum);
    readings["id"] = mac.str();
    readings["name"] = sensorName;
    std::string jsonString{};
    serializeJson(readings, jsonString);

    return jsonString;
}

std::string epochToFormattedDate(unsigned long epochTime)
{
    auto epoch = static_cast<time_t>(epochTime);
    constexpr auto bufSize = 15;
    std::array<char, bufSize> buf{};
    std::strftime(buf.data(), buf.size(), "%Y_%m_%d", std::localtime(&epoch));

    return buf.data();
}

std::string getSHA256(std::string data)
{
    SHA256 hasher;
    hasher.doUpdate(data.c_str());
    std::array<byte, SHA256_SIZE> hash;
    hasher.doFinal(hash.data());

    std::string shaString(SHA256_SIZE * 2, '-');
    constexpr char hexes[]
        = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    for (size_t i = 0; i < hash.size(); ++i)
    {
        shaString[2 * i] = hexes[(hash[i] & 0xF0) >> 4];
        shaString[2 * i + 1] = hexes[hash[i] & 0x0F];
    }

    return shaString;
}

}  // namespace utils
