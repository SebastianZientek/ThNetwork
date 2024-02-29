#pragma once

#include <cstdint>
#include <functional>

#include "common/MacAddr.hpp"

class IEspNow32Adp
{
public:
    enum class Status
    {
        OK,
        FAIL
    };

    using OnSendCb = std::function<void(const MacAddr &, Status status)>;
    using OnRecvCb
        = std::function<void(const MacAddr &mac, const uint8_t *incomingData, uint8_t len)>;

    IEspNow32Adp() = default;
    virtual ~IEspNow32Adp() = default;
    IEspNow32Adp(const IEspNow32Adp &) = default;
    IEspNow32Adp(IEspNow32Adp &&) noexcept = default;
    IEspNow32Adp &operator=(const IEspNow32Adp &) = default;
    IEspNow32Adp &operator=(IEspNow32Adp &&) noexcept = default;

    [[nodiscard]] virtual Status init() const = 0;
    virtual void deinit() const = 0;
    virtual void registerOnSendCb(const OnSendCb &onSendCb) = 0;
    virtual void registerOnRecvCb(const OnRecvCb &onRecvCb) = 0;
    virtual void addPeer(const MacAddr &mac, uint8_t channel) const = 0;
    virtual void deletePeer(const MacAddr &mac) const = 0;
    virtual Status sendData(const MacAddr &mac, uint8_t *data, size_t length) const = 0;
};
