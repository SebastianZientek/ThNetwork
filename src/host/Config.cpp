#include "Config.hpp"

// #include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <FS.h>

#include <optional>

#include "ArduinoJson/Deserialization/DeserializationError.hpp"
#include "common/logger.hpp"

namespace
{
std::pair<DynamicJsonDocument, DeserializationError> jsonFromString(const std::string &str)
{
    constexpr size_t initialCapacity = 32;
    size_t capacity = 0;
    DeserializationError error = DeserializationError::NoMemory;

    while (error == DeserializationError::NoMemory)
    {
        capacity += initialCapacity;
        DynamicJsonDocument json(capacity);
        error = deserializeJson(json, str);
        if (error == DeserializationError::Ok)
        {
            return {json, error};
        }
    }
    return {DynamicJsonDocument(1), DeserializationError::EmptyInput};
}
}  // namespace

bool Config::load(const std::string &data)
{
    auto [config, error] = jsonFromString(data);

    if (error)
    {
        logger::logErr("Deserialize json error");
        return false;
    }

    if (config.containsKey("wifi"))
    {
        auto wifi = config["wifi"];
        if (!wifi.containsKey("ssid") && !wifi.containsKey("pass"))
        {
            logger::logErr("Missing ssid or pass");
            return false;
        }

        m_wifiSsid = std::string(wifi["ssid"]);
        m_wifiPass = std::string(wifi["pass"]);
    }
    else
    {
        logger::logErr("Missing wifi field");
        return false;
    }

    if (config.containsKey("sensors"))
    {
        auto sensors = config["sensors"];
        for (JsonPair keyValue : sensors.as<JsonObject>())
        {
            std::string key = keyValue.key().c_str();
            std::string value = keyValue.value();
            m_sensorsMap[key] = std::string(value);

            logger::logInf("sensor: %s -> %s", key, value);
        }
    }
    else
    {
        return false;
    }

    if (config.containsKey("sensorUpdatePeriodMins"))
    {
        m_sensorUpdatePeriodMins = config["sensorUpdatePeriodMins"];
    }
    else
    {
        return false;
    }

    return true;
}

std::string Config::getExampleConfig()
{
    return R"rawliteral({
    "wifi": {
        "ssid": "",
        "pass": ""
    },
    "sensorUpdatePeriodMins": 1,
    "sensors":{
        "44:17:93:0e:46:26": "Some sensor"
    }
})rawliteral";
}

std::string Config::getWifiSsid() const
{
    return m_wifiSsid;
}

std::string Config::getWifiPass() const
{
    return m_wifiPass;
}

std::map<std::string, std::string> &Config::getSensorsMap()
{
    return m_sensorsMap;
}

std::optional<std::string> Config::getSensorName(const std::string &mac)
{
    auto containsMac = m_sensorsMap.find(mac) != m_sensorsMap.end();
    auto sensorName = containsMac ? std::optional<std::string>{m_sensorsMap.at(mac)} : std::nullopt;
    return sensorName;
}

uint8_t Config::getSensorUpdatePeriodMins() const
{
    return m_sensorUpdatePeriodMins;
}
