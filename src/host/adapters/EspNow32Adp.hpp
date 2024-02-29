#pragma once

#include "IEspNow32Adp.hpp"

class EspNow32Adp : public IEspNow32Adp
{
public:
    [[nodiscard]] Status init() const override;
    void deinit() const override;
    void registerOnSendCb(const OnSendCb &onSendCb) override;
    void registerOnRecvCb(const OnRecvCb &onRecvCb) override;
    void addPeer(const MacAddr &mac, uint8_t channel) const override;
    void deletePeer(const MacAddr &mac) const override;
    Status sendData(const MacAddr &mac, uint8_t *buffer, size_t length) const override;

private:
    static OnSendCb m_onSendCb;  // NOLINT
    static OnRecvCb m_onRecvCb;  // NOLINT
};
