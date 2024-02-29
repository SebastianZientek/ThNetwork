#pragma once

#include <cstddef>
#include <optional>
#include <string>

#include "../adapters/IRaiiFile.hpp"
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

    virtual State load() = 0;
    virtual State save() = 0;

    virtual void setSensorUpdatePeriodMins(uint16_t minutes) = 0;
    [[nodiscard]] virtual uint16_t getSensorUpdatePeriodMins() const = 0;
    virtual void setServerPort(std::size_t port) = 0;
    [[nodiscard]] virtual std::size_t getServerPort() const = 0;
    virtual void setWifiConfig(std::string ssid, std::string pass) = 0;
    virtual std::optional<std::pair<std::string, std::string>> getWifiConfig() = 0;
    virtual void setAdminCredentials(std::string user, std::string pass) = 0;
    [[nodiscard]] virtual std::optional<std::pair<std::string, std::string>> getAdminCredentials()
        const
        = 0;
    [[nodiscard]] virtual std::string getConfigWithoutCredentials() const = 0;
    virtual bool isAvailableSpaceForNextSensor() = 0;
    virtual bool addSensor(IDType identifier, const std::string &name = "") = 0;
    virtual bool removeSensor(IDType identifier) = 0;
    [[nodiscard]] virtual std::string getSensorsMapping() const = 0;
    virtual bool isSensorMapped(IDType identifier) = 0;
};
