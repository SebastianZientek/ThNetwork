#include "Config.hpp"

// #include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <FS.h>

#include <optional>
#include <string>

#include "ArduinoJson/Deserialization/DeserializationError.hpp"
#include "common/MacAddr.hpp"
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

    if (config.containsKey("serverPort"))
    {
        m_serverPort = config["serverPort"].as<int>();
        logger::logInf("Server port: %d", m_serverPort);
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
    "serverPort": 80,
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

std::optional<std::string> Config::getSensorName(IDType identifier)
{
    auto item = std::find_if(m_sensorsMap.begin(), m_sensorsMap.end(),
                             [identifier](auto &item)
                             {
                                 auto mac = MacAddr::strToMac(item.first);
                                 return mac.toUniqueID() == identifier;
                             });

    if (item != m_sensorsMap.end())
    {
        return item->second;
    }
    return std::nullopt;
}

std::optional<IDType> Config::getSensorID(const std::string &sensorName)
{
    auto result
        = std::find_if(m_sensorsMap.begin(), m_sensorsMap.end(),
                       [&sensorName](const auto &item) { return item.second == sensorName; });

    if (result != m_sensorsMap.end())
    {
        auto mac = MacAddr::strToMac(result->first);
        return mac.toUniqueID();
    }

    return std::nullopt;
}

uint8_t Config::getSensorUpdatePeriodMins() const
{
    return m_sensorUpdatePeriodMins;
}

int Config::getServerPort() const
{
    return m_serverPort;
}
