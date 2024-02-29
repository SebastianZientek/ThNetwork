#pragma once

#include <functional>

#include "IEspNow8266Adp.hpp"
#include "common/MacAddr.hpp"

class EspNow8266Adp : public IEspNow8266Adp
{
public:
    [[nodiscard]] Status init() const override;
    void deinit() const override;
    void setRole(Role role) const override;
    void registerOnSendCb(const OnSendCb &onSendCb) override;
    void registerOnRecvCb(const OnRecvCb &onRecvCb) override;
    Status sendData(MacAddr &mac, uint8_t *data, uint8_t length) const override;

private:
    static OnSendCb m_onSendCb;
    static OnRecvCb m_onRecvCb;
};
