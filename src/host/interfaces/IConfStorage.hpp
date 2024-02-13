#pragma once

#include <cstddef>
#include <optional>
#include <string>

#include "IRaiiFile.hpp"
#include "common/types.hpp"

class IConfStorage
{
public:
    enum class State
    {
        OK,
        FAIL
    };

    IConfStorage() = default;
    IConfStorage(const IConfStorage &) = default;
    IConfStorage(IConfStorage &&) = default;
    virtual ~IConfStorage() = default;

    IConfStorage &operator=(const IConfStorage &) = default;
    IConfStorage &operator=(IConfStorage &&) = default;

    virtual State load(IRaiiFile &file) = 0;
    virtual State save(IRaiiFile &file) = 0;

    virtual void setSensorUpdatePeriodMins(std::size_t minutes) = 0;
    virtual std::size_t getSensorUpdatePeriodMins() = 0;
    virtual void setServerPort(std::size_t port) = 0;
    virtual std::size_t getServerPort() = 0;
    virtual void setWifiConfig(std::string ssid, std::string pass) = 0;
    virtual std::optional<std::pair<std::string, std::string>> getWifiConfig() = 0;
    virtual void setAdminCredentials(std::string user, std::string pass) = 0;
    virtual std::optional<std::pair<std::string, std::string>> getAdminCredentials() = 0;
    virtual std::string getConfigWithoutCredentials() = 0;
    virtual bool isAvailableSpaceForNextSensor() = 0;
    virtual bool addSensor(IDType identifier, const std::string &name = "Unnamed") = 0;
    virtual bool removeSensor(IDType identifier) = 0;
    virtual std::string getSensorsMapping() = 0;
    virtual bool isSensorMapped(IDType identifier) = 0;
};