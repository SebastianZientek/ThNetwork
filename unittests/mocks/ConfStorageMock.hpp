#pragma once

#include "interfaces/IConfStorage.hpp"

class ConfStorageMock : public IConfStorage
{
    State load(IRaiiFile &file) override
    {
        return State::OK;
    }

    State save(IRaiiFile &file) override
    {
        return State::OK;
    }

    void setSensorUpdatePeriodMins(std::size_t minutes) override
    {
    }

    std::size_t getSensorUpdatePeriodMins() override
    {
        return 0;
    }

    void setServerPort(std::size_t port) override
    {
    }

    std::size_t getServerPort() override
    {
        return 0;
    }

    void setWifiConfig(std::string ssid, std::string pass) override
    {
    }

    std::optional<std::pair<std::string, std::string>> getWifiConfig() override
    {
        return std::nullopt;
    }

    void setAdminCredentials(std::string user, std::string pass) override
    {
    }

    std::optional<std::pair<std::string, std::string>> getAdminCredentials() override
    {
        return std::nullopt;
    }

    std::string getConfigWithoutCredentials() override
    {
        return "";
    }

    bool isAvailableSpaceForNextSensor() override
    {
        return true;
    }

    bool addSensor(IDType identifier, const std::string &name = "Unnamed") override
    {
        return true;
    }

    bool removeSensor(IDType identifier) override
    {
        return true;
    }

    std::string getSensorsMapping() override
    {
        return "";
    }

    bool isSensorMapped(IDType identifier) override
    {
        return true;
    }
};
