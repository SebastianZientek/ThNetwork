#pragma once

#include <esp_now.h>  // TODO: move to .cpp

#include <cstring>  // TODO: move to .cpp

#include "IEspNowAdp.hpp"
class EspNowAdp : public IEspNowAdp
{
public:
    int init() override
    {
        return esp_now_init();
    }
    void deinit() override
    {
        esp_now_deinit();
    }
    void registerOnSendCb(const OnSendCb &onSendCb) override
    {
        m_onSendCb = onSendCb;
        auto onDataSend = [](const uint8_t *rawMac, esp_now_send_status_t status)
        {
            MacAddr macAddr{};
            std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);

            m_onSendCb(macAddr, status == ESP_OK ? Status::OK : Status::FAIL);
        };
        esp_now_register_send_cb(onDataSend);
    }
    void registerOnRecvCb(const OnRecvCb &onRecvCb) override
    {
        m_onRecvCb = onRecvCb;
        auto onDataRecv = [](const uint8_t *rawMac, const uint8_t *incomingData, int len)
        {
            MacAddr macAddr{};
            std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);

            m_onRecvCb(macAddr, incomingData, len);
        };

        esp_now_register_recv_cb(onDataRecv);
    }
    void addPeer(const MacAddr &mac, uint8_t channel) override
    {
        esp_now_peer_info_t peer = {};
        memcpy(&peer.peer_addr[0], mac.data(), ESP_NOW_ETH_ALEN);
        peer.channel = channel;
        esp_now_add_peer(&peer);
    }
    void deletePeer(const MacAddr &mac) override
    {
        esp_now_del_peer(mac.data());
    }
    Status sendData(const MacAddr &mac, uint8_t* buffer, size_t length) override
    {
        auto state = esp_now_send(mac.data(), buffer, length);
        return state == ESP_OK ? Status::OK : Status::FAIL;
    }

private:
    static OnSendCb m_onSendCb;
    static OnRecvCb m_onRecvCb;
};
