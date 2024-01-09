#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include <string_view>

#include "common/MacAddr.hpp"

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

    void addNewSensor(const MacAddr &macAddr);
    // If name not exists, add to the map (if there is no more than 7 sensors)
    void setSensorName(const MacAddr &mac, std::string_view name);
    void removeSensor(const MacAddr &mac);

    std::pair<std::string, std::string> getCredentials();

    nlohmann::json getConfigWithoutCredentials();

private:
    constexpr static auto configFilePath = "/config.json";
    constexpr static auto maxSensorsNum = 7;
    nlohmann::json m_jsonData;

    void setDefaultData();
};
