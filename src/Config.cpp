#include "Config.hpp"

// #include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <FS.h>

#include "ArduinoJson/Deserialization/DeserializationError.hpp"
#include "logger.hpp"

namespace
{
std::pair<DynamicJsonDocument, DeserializationError> jsonFromString(const String &str)
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

bool Config::load(const String &data)
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

        m_wifiSsid = String(wifi["ssid"]);
        m_wifiPass = String(wifi["pass"]);
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
            String key = keyValue.key().c_str();
            String value = keyValue.value();
            m_sensorsMap[key] = String(value);

            logger::logInfF("sensor: %s -> %s", key.c_str(), value.c_str());
        }
    }

    return true;
}

String Config::getExampleConfig()
{
    return R"rawliteral({
    "wifi": {
        "ssid": "",
        "pass": ""
    },
    "sensors":{
        "44:17:93:0e:46:26": "Some sensor"
    }
})rawliteral";
}

String Config::getWifiSsid() { return m_wifiSsid; }

String Config::getWifiPass() { return m_wifiPass; }

std::map<String, String> &Config::getSensorsMap() { return m_sensorsMap; }
