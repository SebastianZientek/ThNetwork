#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>

#include "RaiiFile.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"

class ConfStorage
{
public:
    enum class State
    {
        OK,
        FAIL
    };

    template <typename RaiiFileT>
    State load(RaiiFileT &file)
    {
        std::string data = file->readString().c_str();
        try
        {
            m_jsonData = nlohmann::json::parse(data);
        }
        catch (nlohmann::json::parse_error err)
        {
            logger::logErr("Can't parse json data, %s", err.what());
            return State::FAIL;
        }

        return State::OK;
    }

    template <typename RaiiFileT>
    State save(RaiiFileT &file)
    {
        try
        {
            auto data = m_jsonData.dump();
            file->print(data.c_str());
        }
        catch (nlohmann::json::type_error err)
        {
            logger::logErr("Can't dump json data of configuration file, %s", err.what());
            return State::FAIL;
        }

        return State::OK;
    }

    void setDefault();

    void setSensorUpdatePeriodMins(std::size_t minutes);
    std::size_t getSensorUpdatePeriodMins();
    void setServerPort(std::size_t port);
    std::size_t getServerPort();
    void setWifiConfig(std::string ssid, std::string pass);
    std::optional<std::pair<std::string, std::string>> getWifiConfig();
    void setAdminCredentials(std::string user, std::string pass);
    std::optional<std::pair<std::string, std::string>> getAdminCredentials();

    nlohmann::json getConfigWithoutCredentials();

    bool isAvailableSpaceForNextSensor();
    bool addSensor(IDType identifier, const std::string &name = "Unnamed");
    bool removeSensor(IDType identifier);
    nlohmann::json getSensorsMapping();

private:
    // Limited because of space for readings
    constexpr static auto maxSensorsNum = 7;
    nlohmann::json m_jsonData{};
};
