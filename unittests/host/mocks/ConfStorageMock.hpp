#pragma once
#include <CppUTestExt/MockSupport.h>

#include "interfaces/IConfStorage.hpp"

class ConfStorageMock : public IConfStorage
{
    State load() override
    {
        static auto defaultState = State::OK;
        mock().actualCall("ConfStorageMock::load");
        auto *returnVal = mock().returnPointerValueOrDefault(&defaultState);

        return *static_cast<State *>(returnVal);
    }

    State save() override
    {
        static auto defaultState = State::OK;
        mock().actualCall("ConfStorageMock::save");
        auto *returnVal = mock().returnPointerValueOrDefault(&defaultState);

        return *static_cast<State *>(returnVal);
    }

    void setSensorUpdatePeriodMins(std::size_t minutes) override
    {
        mock()
            .actualCall("ConfStorageMock::setSensorUpdatePeriodMins")
            .withParameter("minutes", minutes);
    }

    std::size_t getSensorUpdatePeriodMins() override
    {
        return mock()
            .actualCall("ConfStorageMock::getSensorUpdatePeriodMins")
            .returnUnsignedLongIntValueOrDefault(0);
    }

    void setServerPort(std::size_t port) override
    {
        mock().actualCall("ConfStorageMock::setServerPort").withParameter("port", port);
    }

    std::size_t getServerPort() override
    {
        return mock()
            .actualCall("ConfStorageMock::getServerPort")
            .returnUnsignedLongIntValueOrDefault(0);
    }

    void setWifiConfig(std::string ssid, std::string pass) override
    {
        mock()
            .actualCall("ConfStorageMock::setWifiConfig")
            .withStringParameter("ssid", ssid.c_str())
            .withStringParameter("pass", pass.c_str());
    }

    std::optional<std::pair<std::string, std::string>> getWifiConfig() override
    {
        using ReturnType = std::optional<std::pair<std::string, std::string>>;
        static auto defaultState = std::nullopt;
        mock().actualCall("ConfStorageMock::getWifiConfig");
        auto *returnVal = mock().returnPointerValueOrDefault(&defaultState);

        return *static_cast<ReturnType *>(returnVal);
    }

    void setAdminCredentials(std::string user, std::string pass) override
    {
        mock()
            .actualCall("ConfStorageMock::setAdminCredentials")
            .withStringParameter("user", user.c_str())
            .withStringParameter("pass", pass.c_str());
    }

    std::optional<std::pair<std::string, std::string>> getAdminCredentials() override
    {
        using ReturnType = std::optional<std::pair<std::string, std::string>>;
        static auto defaultState = std::nullopt;
        mock().actualCall("ConfStorageMock::getAdminCredentials");
        auto *returnVal = mock().returnPointerValueOrDefault(&defaultState);

        return *static_cast<ReturnType *>(returnVal);
    }

    std::string getConfigWithoutCredentials() override
    {
        return mock()
            .actualCall("ConfStorageMock::getConfigWithoutCredentials")
            .returnStringValueOrDefault("{}");
    }

    bool isAvailableSpaceForNextSensor() override
    {
        return mock()
            .actualCall("ConfStorageMock::isAvailableSpaceForNextSensor")
            .returnBoolValueOrDefault(true);
    }

    bool addSensor(IDType identifier, const std::string &name = "Unnamed") override
    {
        return mock()
            .actualCall("ConfStorageMock::addSensor")
            .withUnsignedIntParameter("identifier", identifier)
            .withStringParameter("name", name.c_str())
            .returnBoolValueOrDefault(true);
    }

    bool removeSensor(IDType identifier) override
    {
        return mock()
            .actualCall("ConfStorageMock::removeSensor")
            .withUnsignedIntParameter("identifier", identifier)
            .returnBoolValueOrDefault(true);
    }

    std::string getSensorsMapping() override
    {
        return mock()
            .actualCall("ConfStorageMock::getSensorsMapping")
            .returnStringValueOrDefault("");
    }

    bool isSensorMapped(IDType identifier) override
    {
        return mock()
            .actualCall("ConfStorageMock::isSensorMapped")
            .withUnsignedIntParameter("identifier", identifier)
            .returnBoolValueOrDefault(true);
    }
};
