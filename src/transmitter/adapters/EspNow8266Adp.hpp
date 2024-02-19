#pragma once

#include <functional>

#include "IEspNow8266Adp.hpp"
#include "common/MacAddr.hpp"

class EspNow8266Adp : public IEspNow8266Adp
{
public:
    Status init() override;
    void deinit() override;
    void setRole(Role role) override;
    void registerOnSendCb(const OnSendCb &onSendCb) override;
    void registerOnRecvCb(const OnRecvCb &onRecvCb) override;
    Status sendData(MacAddr &mac, uint8_t *data, size_t length) override;

private:
    static OnSendCb m_onSendCb;
    static OnRecvCb m_onRecvCb;
};
