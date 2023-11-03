#include "utils.hpp"

#include <ArduinoJson.h>

#include "ArduinoJson/Document/StaticJsonDocument.hpp"
#include "ArduinoJson/Json/JsonSerializer.hpp"

namespace
{
String floatToOneDecimalStr(float val)
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

String readingsToJsonString(
    float temp, float hum, MacAddr mac, const String &sensorName, unsigned long epochTime)
{
    constexpr auto initialBufferSize = 5;

    StaticJsonDocument<readingsJsonSize> readings{};  // NOLINT
    readings["epochTime"] = epochTime;
    readings["temperature"] = floatToOneDecimalStr(temp);
    readings["humidity"] = floatToOneDecimalStr(hum);
    readings["id"] = mac.str();
    readings["name"] = sensorName;
    String jsonString{};
    serializeJson(readings, jsonString);

    return jsonString;
}
}  // namespace utils
