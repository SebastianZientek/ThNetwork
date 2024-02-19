#pragma once

#include <functional>

#include "common/MacAddr.hpp"

class IEspNow8266Adp
{
public:
    enum class Status
    {
        OK,
        FAIL
    };

    enum class MsgHandleStatus
    {
        ACCEPTED,
        REJECTED
    };

    enum class Role
    {
        IDLE,
        CONTROLLER,
        SLAVE,
        COMBO,
        MAX,
    };

    using OnSendCb = std::function<void(const MacAddr &, Status status)>;
    using OnRecvCb = std::function<MsgHandleStatus(
        const MacAddr &mac, const uint8_t *incomingData, uint8_t len)>;

    IEspNow8266Adp() = default;
    virtual ~IEspNow8266Adp() = default;
    IEspNow8266Adp(const IEspNow8266Adp &) = default;
    IEspNow8266Adp(IEspNow8266Adp &&) noexcept = default;
    IEspNow8266Adp &operator=(const IEspNow8266Adp &) = default;
    IEspNow8266Adp &operator=(IEspNow8266Adp &&) noexcept = default;

    virtual Status init() = 0;
    virtual void deinit() = 0;
    virtual void setRole(Role role) = 0;
    virtual void registerOnSendCb(const OnSendCb &onSendCb) = 0;
    virtual void registerOnRecvCb(const OnRecvCb &onRecvCb) = 0;
    virtual Status sendData(MacAddr &mac, uint8_t *data, size_t length) = 0;
};
