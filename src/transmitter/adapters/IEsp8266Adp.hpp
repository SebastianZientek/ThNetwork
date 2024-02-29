#pragma once

class IEsp8266Adp
{
public:
    IEsp8266Adp() = default;
    virtual ~IEsp8266Adp() = default;
    IEsp8266Adp(const IEsp8266Adp &) = default;
    IEsp8266Adp(IEsp8266Adp &&) noexcept = default;
    IEsp8266Adp &operator=(const IEsp8266Adp &) = default;
    IEsp8266Adp &operator=(IEsp8266Adp &&) noexcept = default;

    virtual void feedWatchdog() const = 0;
    virtual void yield() const = 0;
    [[nodiscard]] virtual bool isResetReasonDeepSleepAwake() const = 0;
};
