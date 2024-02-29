#pragma once

#include <cstdint>
#include <string>

class IWifi8266Adp
{
public:
    IWifi8266Adp() = default;
    virtual ~IWifi8266Adp() = default;
    IWifi8266Adp(const IWifi8266Adp &) = default;
    IWifi8266Adp(IWifi8266Adp &&) noexcept = default;
    IWifi8266Adp &operator=(const IWifi8266Adp &) = default;
    IWifi8266Adp &operator=(IWifi8266Adp &&) noexcept = default;

    virtual void setModeSta() const = 0;
    virtual void setChannel(uint8_t channel) const = 0;
    virtual void disconnect() const = 0;
    virtual uint8_t *macAddress(uint8_t *mac) const = 0;
    [[nodiscard]] virtual std::string macAddress() const = 0;
};
