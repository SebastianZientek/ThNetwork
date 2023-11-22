#pragma once

#include <Arduino.h>

#include <map>
#include <optional>
#include <string>

class Config
{
public:
    bool load(const std::string &data);
    std::string getExampleConfig();

    std::string getWifiSsid();
    std::string getWifiPass();
    std::map<std::string, std::string> &getSensorsMap();
    std::optional<std::string> getSensorName(const std::string &mac);
    [[nodiscard]] uint8_t getSensorUpdatePeriodMins() const;

private:
    std::string m_wifiSsid;
    std::string m_wifiPass;
    std::map<std::string, std::string> m_sensorsMap;
    uint8_t m_sensorUpdatePeriodMins;
};
