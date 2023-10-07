#include "Config.hpp"

#include <FS.h>

bool Config::load(String data)
{
    JSONVar config = JSON.parse(data);

    if (config.hasOwnProperty("wifi"))
    {
        auto wifi = config["wifi"];
        if (!wifi.hasOwnProperty("ssid") && !wifi.hasOwnProperty("pass"))
        {
            Serial.println("ERROR: Missing ssid or pass");
            return false;
        }

        m_wifiSsid = String(wifi["ssid"]);
        m_wifiPass = String(wifi["pass"]);
    }
    else
    {
        Serial.println("ERROR: Missing wifi field");
        return false;
    }

    if (config.hasOwnProperty("sensors"))
    {
        auto sensors = config["sensors"];
        auto keys = sensors.keys();
        for (int i = 0; i < keys.length(); ++i)
        {
            auto key = keys[i];
            auto value = sensors[key];
            m_sensorsMap[key] = String(value);
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

std::map<String, String> Config::getSensorsMap()
{
    return m_sensorsMap;
}