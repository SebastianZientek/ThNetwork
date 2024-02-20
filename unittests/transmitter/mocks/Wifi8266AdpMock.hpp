#pragma once

#include "adapters/IWifi8266Adp.hpp"

class Wifi8266AdpMock : public IWifi8266Adp
{
public:
    void setModeSta() override
    {
        mock("Wifi8266AdpMock").actualCall("setModeSta");
    }

    void setChannel(uint8_t channel) override
    {
        mock("Wifi8266AdpMock").actualCall("setChannel").withParameter("channel", channel);
    }

    void disconnect() override
    {
        mock("Wifi8266AdpMock").actualCall("disconnect");
    }

    uint8_t *macAddress(uint8_t *mac) override
    {
        mock("Wifi8266AdpMock").actualCall("macAddress");
        return nullptr;
    }

    std::string macAddress() override
    {
        return mock("Wifi8266AdpMock").actualCall("macAddress").returnStringValueOrDefault("");
    }
};
