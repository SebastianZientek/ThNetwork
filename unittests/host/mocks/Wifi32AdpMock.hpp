#pragma once

#include <CppUTestExt/MockSupport.h>

#include <cinttypes>

#include "adapters/IWifi32Adp.hpp"

class Wifi32AdpMock : public IWifi32Adp
{
public:
    void init(const std::string &ssid, const std::string &pass) const override
    {
        mock("Wifi32AdpMock")
            .actualCall("init")
            .withStringParameter("ssid", ssid.c_str())
            .withStringParameter("pass", pass.c_str());
    }

    void disconnect() const override
    {
        mock("Wifi32AdpMock").actualCall("disconnect");
    }

    void setMode(const Mode &mode) const override
    {
        mock("Wifi32AdpMock").actualCall("setMode").withParameter("mode", static_cast<int>(mode));
    }

    [[nodiscard]] Status getStatus() const override
    {
        static auto defaultState = Status::IDLE_STATUS;
        mock("Wifi32AdpMock").actualCall("getStatus");
        auto *returnVal = mock("Wifi32AdpMock").returnPointerValueOrDefault(&defaultState);

        return *static_cast<Status *>(returnVal);
    }

    [[nodiscard]] std::string getSsid() const override
    {
        return mock("Wifi32AdpMock").actualCall("getSsid").returnStringValueOrDefault("ssid");
    }

    [[nodiscard]] std::string getLocalIp() const override
    {
        return mock("Wifi32AdpMock")
            .actualCall("getLocalIp")
            .returnStringValueOrDefault("192.168.0.1");
    }

    [[nodiscard]] std::string getMacAddr() const override
    {
        return mock("Wifi32AdpMock")
            .actualCall("getMacAddr")
            .returnStringValueOrDefault("AA:BB:CC:DD:EE:FF");
    }

    [[nodiscard]] std::size_t getChannel() const override
    {
        return mock("Wifi32AdpMock").actualCall("getChannel").returnUnsignedIntValueOrDefault(1);
    }

    [[nodiscard]] std::array<uint8_t, macAddrDigits> getSoftApMacAddr() const override
    {
        mock("Wifi32AdpMock").actualCall("getSoftApMacAddr");

        std::array<uint8_t, macAddrDigits> defaultMacArr{};
        auto *returnVal = mock("Wifi32AdpMock").returnPointerValueOrDefault(&defaultMacArr);

        return *static_cast<std::array<uint8_t, macAddrDigits> *>(returnVal);
    }

    void softAp(const std::string &ssid, const std::string &pass = "") const override
    {
        mock("Wifi32AdpMock")
            .actualCall("softAp")
            .withStringParameter("ssid", ssid.c_str())
            .withStringParameter("pass", pass.c_str());
    }

    [[nodiscard]] std::string getSoftApIp() const override
    {
        return mock("Wifi32AdpMock")
            .actualCall("getSoftApIp")
            .returnStringValueOrDefault("192.168.0.1");
    }
};
