#pragma once

#include "adapters/IWifi8266Adp.hpp"

class Wifi8266AdpMock : public IWifi8266Adp
{
public:
    void setModeSta() const override
    {
        mock("Wifi8266AdpMock").actualCall("setModeSta");
    }

    void setChannel(uint8_t channel) const override
    {
        mock("Wifi8266AdpMock").actualCall("setChannel").withParameter("channel", channel);
    }

    void disconnect() const override
    {
        mock("Wifi8266AdpMock").actualCall("disconnect");
    }

    uint8_t *macAddress(uint8_t *mac) const override
    {
        mock("Wifi8266AdpMock").actualCall("macAddress");
        return nullptr;
    }

    [[nodiscard]] std::string macAddress() const override
    {
        return mock("Wifi8266AdpMock").actualCall("macAddress").returnStringValueOrDefault("");
    }
};
