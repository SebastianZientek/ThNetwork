#pragma once

#include "IWifi8266Adp.hpp"

class Wifi8266Adp : public IWifi8266Adp
{
public:
    void setModeSta() const override;
    void setChannel(uint8_t channel) const override;
    void disconnect() const override;
    uint8_t *macAddress(uint8_t *mac) const override;
    [[nodiscard]] std::string macAddress() const override;
};
