#pragma once

#include <cinttypes>
#include <string>

#include "IWifi32Adp.hpp"

class Wifi32Adp : public IWifi32Adp
{
public:
    void init(std::string ssid, std::string pass) const override;
    void disconnect() const override;
    void setMode(const Mode &mode) const override;
    [[nodiscard]] Status getStatus() const override;
    [[nodiscard]] std::string getSsid() const override;
    [[nodiscard]] std::string getLocalIp() const override;
    [[nodiscard]] std::string getMacAddr() const override;
    [[nodiscard]] std::size_t getChannel() const override;
    [[nodiscard]] std::array<uint8_t, macAddrDigits> getSoftApMacAddr() const override;
    void softAp(std::string ssid, std::string pass = "") const override;
    [[nodiscard]] std::string getSoftApIp() const override;
};
