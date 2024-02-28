#pragma once

#include <cinttypes>
#include <string>

#include "IWifi32Adp.hpp"

class Wifi32Adp : public IWifi32Adp
{
public:
    void init(std::string ssid, std::string pass) override;
    void disconnect() override;
    void setMode(const Mode &mode) override;
    Status getStatus() override;
    std::string getSsid() override;
    std::string getLocalIp() override;
    std::string getMacAddr() override;
    std::size_t getChannel() override;
    std::array<uint8_t, macAddrDigits> getSoftApMacAddr() override;
    void softAp(std::string ssid, std::string pass = "") override;
    std::string getSoftApIp() override;
};
