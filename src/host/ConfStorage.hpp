#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include <string_view>

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

    std::pair<std::string, std::string> getCredentials();

    nlohmann::json getConfigWithoutCredentials();

private:
    constexpr static auto configFilePath = "/config.json";
    constexpr static auto maxSensorsNum = 7;
    nlohmann::json m_jsonData;

    void setDefaultData();
};
