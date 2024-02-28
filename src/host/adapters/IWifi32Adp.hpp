#pragma once

#include <array>
#include <cinttypes>
#include <string>

class IWifi32Adp
{
public:
    enum class Status
    {
        ERROR,
        NO_SHIELD,
        IDLE_STATUS,
        NO_SSID_AVAIL,
        SCAN_COMPLETED,
        CONNECTED,
        CONNECT_FAILED,
        CONNECTION_LOST,
        DISCONNECTED,
    };

    enum class Mode
    {
        OFF,
        STA,
        AP,
        AP_STA
    };

    constexpr static auto macAddrDigits = 6;

    IWifi32Adp() = default;
    virtual ~IWifi32Adp() = default;
    IWifi32Adp(const IWifi32Adp &) = default;
    IWifi32Adp(IWifi32Adp &&) noexcept = default;
    IWifi32Adp &operator=(const IWifi32Adp &) = default;
    IWifi32Adp &operator=(IWifi32Adp &&) noexcept = default;

    virtual void init(std::string ssid, std::string pass) = 0;
    virtual void disconnect() = 0;
    virtual void setMode(const Mode &mode) = 0;
    virtual Status getStatus() = 0;
    virtual std::string getSsid() = 0;
    virtual std::string getLocalIp() = 0;
    virtual std::string getMacAddr() = 0;
    virtual std::size_t getChannel() = 0;
    virtual std::array<uint8_t, macAddrDigits> getSoftApMacAddr() = 0;
    virtual void softAp(std::string ssid, std::string pass = "") = 0;
    virtual std::string getSoftApIp() = 0;
};
