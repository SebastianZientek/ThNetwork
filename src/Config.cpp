#include "Config.hpp"

// #include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <FS.h>

#include "logger.hpp"

bool Config::load(String data)
{
    DynamicJsonDocument config(1000);
    DeserializationError error = deserializeJson(config, data);

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

String Config::getWifiSsid()
{
    return m_wifiSsid;
}

String Config::getWifiPass()
{
    return m_wifiPass;
}

std::map<String, String> &Config::getSensorsMap()
{
    return m_sensorsMap;
}
