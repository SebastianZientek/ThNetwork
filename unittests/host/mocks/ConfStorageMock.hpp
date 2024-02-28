#pragma once
#include <CppUTestExt/MockSupport.h>

#include "interfaces/IConfStorage.hpp"

class ConfStorageMock : public IConfStorage
{
    State load() override
    {
        static auto defaultState = State::OK;
        mock("ConfStorageMock").actualCall("load");
        auto *returnVal = mock("ConfStorageMock").returnPointerValueOrDefault(&defaultState);

        return *static_cast<State *>(returnVal);
    }

    State save() override
    {
        static auto defaultState = State::OK;
        mock("ConfStorageMock").actualCall("save");
        auto *returnVal = mock("ConfStorageMock").returnPointerValueOrDefault(&defaultState);

        return *static_cast<State *>(returnVal);
    }

    void setSensorUpdatePeriodMins(uint16_t minutes) override
    {
        mock("ConfStorageMock")
            .actualCall("setSensorUpdatePeriodMins")
            .withParameter("minutes", minutes);
    }

    uint16_t getSensorUpdatePeriodMins() override
    {
        return mock("ConfStorageMock")
            .actualCall("getSensorUpdatePeriodMins")
            .returnUnsignedLongIntValueOrDefault(0);
    }

    void setServerPort(std::size_t port) override
    {
        mock("ConfStorageMock").actualCall("setServerPort").withParameter("port", port);
    }

    std::size_t getServerPort() override
    {
        return mock("ConfStorageMock")
            .actualCall("getServerPort")
            .returnUnsignedLongIntValueOrDefault(0);
    }

    void setWifiConfig(std::string ssid, std::string pass) override
    {
        mock("ConfStorageMock")
            .actualCall("setWifiConfig")
            .withStringParameter("ssid", ssid.c_str())
            .withStringParameter("pass", pass.c_str());
    }

    std::optional<std::pair<std::string, std::string>> getWifiConfig() override
    {
        using ReturnType = std::optional<std::pair<std::string, std::string>>;
        static auto defaultState = std::nullopt;
        mock("ConfStorageMock").actualCall("getWifiConfig");
        auto *returnVal = mock("ConfStorageMock").returnPointerValueOrDefault(&defaultState);

        return *static_cast<ReturnType *>(returnVal);
    }

    void setAdminCredentials(std::string user, std::string pass) override
    {
        mock("ConfStorageMock")
            .actualCall("setAdminCredentials")
            .withStringParameter("user", user.c_str())
            .withStringParameter("pass", pass.c_str());
    }

    std::optional<std::pair<std::string, std::string>> getAdminCredentials() override
    {
        using ReturnType = std::optional<std::pair<std::string, std::string>>;
        static auto defaultState = std::nullopt;
        mock("ConfStorageMock").actualCall("getAdminCredentials");
        auto *returnVal = mock("ConfStorageMock").returnPointerValueOrDefault(&defaultState);

        return *static_cast<ReturnType *>(returnVal);
    }

    std::string getConfigWithoutCredentials() override
    {
        return mock("ConfStorageMock")
            .actualCall("getConfigWithoutCredentials")
            .returnStringValueOrDefault("{}");
    }

    bool isAvailableSpaceForNextSensor() override
    {
        return mock("ConfStorageMock")
            .actualCall("isAvailableSpaceForNextSensor")
            .returnBoolValueOrDefault(true);
    }

    bool addSensor(IDType identifier, const std::string &name = "Unnamed") override
    {
        return mock("ConfStorageMock")
            .actualCall("addSensor")
            .withUnsignedIntParameter("identifier", identifier)
            .withStringParameter("name", name.c_str())
            .returnBoolValueOrDefault(true);
    }

    bool removeSensor(IDType identifier) override
    {
        return mock("ConfStorageMock")
            .actualCall("removeSensor")
            .withUnsignedIntParameter("identifier", identifier)
            .returnBoolValueOrDefault(true);
    }

    std::string getSensorsMapping() override
    {
        return mock("ConfStorageMock")
            .actualCall("getSensorsMapping")
            .returnStringValueOrDefault("");
    }

    bool isSensorMapped(IDType identifier) override
    {
        return mock("ConfStorageMock")
            .actualCall("isSensorMapped")
            .withUnsignedIntParameter("identifier", identifier)
            .returnBoolValueOrDefault(true);
    }
};
