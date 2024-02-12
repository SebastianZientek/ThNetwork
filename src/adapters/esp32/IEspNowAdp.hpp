#pragma once

#include <cstdint>
#include <functional>

#include "common/MacAddr.hpp"

class IEspNowAdp
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

    IEspNowAdp() = default;
    virtual ~IEspNowAdp() = default;
    IEspNowAdp(const IEspNowAdp &) = default;
    IEspNowAdp(IEspNowAdp &&) noexcept = default;
    IEspNowAdp &operator=(const IEspNowAdp &) = default;
    IEspNowAdp &operator=(IEspNowAdp &&) noexcept = default;

    virtual int init() = 0;
    virtual void deinit() = 0;
    virtual void registerOnSendCb(const OnSendCb &onSendCb) = 0;
    virtual void registerOnRecvCb(const OnRecvCb &onRecvCb) = 0;
    virtual void addPeer(const MacAddr &mac, uint8_t channel) = 0;
    virtual void deletePeer(const MacAddr &mac) = 0;
    virtual Status sendData(const MacAddr &mac, uint8_t* data, size_t length) = 0;
};
