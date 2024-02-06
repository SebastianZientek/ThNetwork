#pragma once

#include <cstddef>
#include <nlohmann/json.hpp>
#include <optional>

#include "common/types.hpp"

class IConfStorage
{
public:
    enum class State
    {
        OK,
        FAIL
    };

    virtual ~IConfStorage() = default;

    virtual void setSensorUpdatePeriodMins(std::size_t minutes) = 0;
    virtual std::size_t getSensorUpdatePeriodMins() = 0;
    virtual void setServerPort(std::size_t port) = 0;
    virtual std::size_t getServerPort() = 0;
    virtual void setWifiConfig(std::string ssid, std::string pass) = 0;
    virtual std::optional<std::pair<std::string, std::string>> getWifiConfig() = 0;
    virtual void setAdminCredentials(std::string user, std::string pass) = 0;
    virtual std::optional<std::pair<std::string, std::string>> getAdminCredentials() = 0;
    virtual nlohmann::json getConfigWithoutCredentials() = 0;
    virtual bool isAvailableSpaceForNextSensor() = 0;
    virtual bool addSensor(IDType identifier, const std::string &name = "Unnamed") = 0;
    virtual bool removeSensor(IDType identifier) = 0;
    virtual nlohmann::json getSensorsMapping() = 0;
};
