#pragma once

#include <Arduino.h>

#include <map>
#include <optional>

class Config
{
public:
    bool load(const String &data);
    String getExampleConfig();

    String getWifiSsid();
    String getWifiPass();
    std::map<String, String> &getSensorsMap();
    std::optional<String> getSensorName(const String &mac);
    [[nodiscard]] uint8_t getSensorUpdatePeriodMins() const;

private:
    String m_wifiSsid;
    String m_wifiPass;
    std::map<String, String> m_sensorsMap;
    uint8_t m_sensorUpdatePeriodMins;
};
