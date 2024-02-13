#include "EspNow32Adp.hpp"

#include <esp_now.h>

#include <cstring>

EspNow32Adp::OnSendCb EspNow32Adp::m_onSendCb;  // NOLINT
EspNow32Adp::OnRecvCb EspNow32Adp::m_onRecvCb;  // NOLINT

EspNow32Adp::Status EspNow32Adp::init()
{
    return esp_now_init() == ESP_OK ? EspNow32Adp::Status::OK : EspNow32Adp::Status::FAIL;
}

void EspNow32Adp::deinit()
{
    esp_now_deinit();
}

void EspNow32Adp::registerOnSendCb(const OnSendCb &onSendCb)
{
    m_onSendCb = onSendCb;
    auto onDataSend = [](const uint8_t *rawMac, esp_now_send_status_t status)
    {
        MacAddr macAddr{};
        std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);

        m_onSendCb(macAddr, status == ESP_OK ? EspNow32Adp::Status::OK
                                             : EspNow32Adp::Status::FAIL);
    };
    esp_now_register_send_cb(onDataSend);
}

void EspNow32Adp::registerOnRecvCb(const OnRecvCb &onRecvCb)
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

void EspNow32Adp::addPeer(const MacAddr &mac, uint8_t channel)
{
    esp_now_peer_info_t peer = {};
    memcpy(&peer.peer_addr[0], mac.data(), ESP_NOW_ETH_ALEN);
    peer.channel = channel;
    esp_now_add_peer(&peer);
}

void EspNow32Adp::deletePeer(const MacAddr &mac)
{
    esp_now_del_peer(mac.data());
}

EspNow32Adp::Status EspNow32Adp::sendData(const MacAddr &mac, uint8_t *buffer, size_t length)
{
    auto state = esp_now_send(mac.data(), buffer, length);
    return state == ESP_OK ? EspNow32Adp::Status::OK
                           : EspNow32Adp::Status::FAIL;
}
