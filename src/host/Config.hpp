#pragma once

#include <Arduino.h>

#include <map>
#include <optional>
#include <string>
#include "common/MacAddr.hpp"
#include "common/types.hpp"

class Config
{
public:
    bool load(const std::string &data);
    std::string getExampleConfig();

    [[nodiscard]] std::string getWifiSsid() const;
    [[nodiscard]] std::string getWifiPass() const;
    std::map<std::string, std::string> &getSensorsMap();
    std::optional<std::string> getSensorName(IDType id);
    std::optional<IDType> getSensorID(const std::string& sensorName);
    [[nodiscard]] uint8_t getSensorUpdatePeriodMins() const;
    [[nodiscard]] int getServerPort() const;

private:
    std::string m_wifiSsid{};
    std::string m_wifiPass{};
    std::map<std::string, std::string> m_sensorsMap{};
    uint8_t m_sensorUpdatePeriodMins{};
    int m_serverPort{80};  // NOLINT
};
