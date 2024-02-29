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

    virtual void init(const std::string &ssid, const std::string &pass) const = 0;
    virtual void disconnect() const = 0;
    virtual void setMode(const Mode &mode) const = 0;
    [[nodiscard]] virtual Status getStatus() const = 0;
    [[nodiscard]] virtual std::string getSsid() const = 0;
    [[nodiscard]] virtual std::string getLocalIp() const = 0;
    [[nodiscard]] virtual std::string getMacAddr() const = 0;
    [[nodiscard]] virtual std::size_t getChannel() const = 0;
    [[nodiscard]] virtual std::array<uint8_t, macAddrDigits> getSoftApMacAddr() const = 0;
    virtual void softAp(const std::string &ssid, const std::string &pass = "") const = 0;
    [[nodiscard]] virtual std::string getSoftApIp() const = 0;
};
