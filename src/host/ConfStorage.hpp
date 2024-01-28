#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>
#include <FS.h>

#include "common/types.hpp"

class ConfStorage
{
public:
    enum class State
    {
        OK,
        FAIL
    };

    ConfStorage(fs::FS filesystem, std::string path);
    State load();
    State save();
    State reset();

    void setWifiConfig(std::string ssid, std::string pass);
    std::optional<std::pair<std::string, std::string>> getWifiConfig();

    std::pair<std::string, std::string> getCredentials();
    std::string getSensorIDsToNamesJsonStr();
    std::size_t getSensorUpdatePeriodMins();
    std::size_t getServerPort();

    nlohmann::json getConfigWithoutCredentials();

private:
    constexpr static auto maxSensorsNum = 7;
    std::string m_configFilePath{};
    nlohmann::json m_jsonData{};
    fs::FS m_filesystem;

    void setDefaultData();
};
