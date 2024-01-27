#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>

#include "common/types.hpp"

class ConfStorage
{
public:
    enum class State
    {
        OK,
        FAIL
    };

    ConfStorage();
    State load();
    State save();
    State reset();

    void setWifiConfig(std::string ssid, std::string pass);
    std::pair<std::string, std::string> getCredentials();
    std::optional<std::pair<std::string, std::string>> getWifiConfig();
    std::string getSensorName(IDType identifier);
    std::string getSensorIDsToNamesJsonStr();

    nlohmann::json getConfigWithoutCredentials();

private:
    constexpr static auto configFilePath = "/config.json";
    constexpr static auto maxSensorsNum = 7;
    nlohmann::json m_jsonData{};

    void setDefaultData();
};
