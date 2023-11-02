#include "EspNow.hpp"

#include <WiFi.h>

#include <array>

#include "NTPClient.h"
#include "THMessage.hpp"
#include "logger.hpp"

constexpr auto macSize = 6;
constexpr auto msgSignatureSize = 4;
constexpr std::array<uint8_t, macSize> broadcastAddress{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
constexpr std::array<uint8_t, 4> msgSignature{'T', 'H', 'D', 'T'};

EspNow::EspNow(NTPClient &ntpClient)
    : m_ntpClient(ntpClient)
{
}

void EspNow::init(const NewReadingsCb &newReadingsCb)
{
    if (esp_now_init() != ESP_OK)
    {
        logger::logErr("Init ESP NOW issue");
        return;
    }

    m_newReadingsCb = newReadingsCb;
    setOnDataRecvCb();
    setOnDataSendCb();
}

void EspNow::onDataRecv(MacAddr mac, const uint8_t *incomingData, int len)
{
    THMessage recvMsg{};
    memcpy(&recvMsg, incomingData, sizeof(recvMsg));
    switch (recvMsg.type)
    {
    case PAIR_REQ:
        logger::logInf("PAIR_REQ received");
        addPeer(mac, WiFi.channel());
        sendPairOK(mac);
        break;
    case PAIR_OK:
        logger::logErr("PAIR_OK received, but this message should be handled by sensor");
        break;
    case TH_DATA:
        logger::logInf("TH_DATA received");
        logger::logInf(m_ntpClient.getFormattedTime());
        logger::logInfF("T: %.1f, H: %.1f", recvMsg.temperature, recvMsg.humidity);

        m_newReadingsCb(recvMsg.temperature, recvMsg.humidity, mac, m_ntpClient.getEpochTime());
        break;
    }
}

void EspNow::onDataSend(MacAddr mac, esp_now_send_status_t status)
{
    logger::logInf("Last Packet Send Status: ");
    if (status == 0)
    {
        logger::logInfF("Delivery success: %s", String(mac).c_str());
    }
    else
    {
        logger::logErr("Delivery fail");
    }
}

void EspNow::setOnDataRecvCb()
{
    static auto onRecvDataThis = [this](MacAddr mac, const uint8_t *incomingData, int len)
    { this->onDataRecv(std::move(mac), incomingData, len); };
    auto onDataRecv = [](const uint8_t *macAddr, const uint8_t *incomingData, int len)
    { onRecvDataThis(macAddr, incomingData, len); };

    esp_now_register_recv_cb(onDataRecv);
}

void EspNow::setOnDataSendCb()
{
    static auto onDataSendThis = [this](MacAddr mac, esp_now_send_status_t status)
    { this->onDataSend(std::move(mac), status); };
    auto onDataSend = [](const uint8_t *macAddr, esp_now_send_status_t status)
    { onDataSendThis(macAddr, status); };

    esp_now_register_send_cb(onDataSend);
}

void EspNow::addPeer(MacAddr mac, uint8_t channel)
{
    esp_now_peer_info_t peer = {};
    memcpy(&peer.peer_addr[0], mac, ESP_NOW_ETH_ALEN);
    peer.channel = channel;
    esp_now_add_peer(&peer);
}

void EspNow::sendPairOK(MacAddr mac)
{
    THMessage msg{};
    msg.type = PAIR_OK;
    msg.channel = WiFi.channel();
    WiFi.softAPmacAddress(&msg.mac[0]);
    std::copy(msgSignature.begin(), msgSignature.end(), &msg.signature[0]);
    auto state = esp_now_send(mac, reinterpret_cast<uint8_t *>(&msg), sizeof(msg));  // NOLINT

    if (state != ESP_OK)
    {
        logger::logErrF("esp_now_send error, code: %d", state);
    }
}
