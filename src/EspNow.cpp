#include "EspNow.hpp"

#include <WiFi.h>

#include <array>

#include "THMessage.hpp"
#include "logger.hpp"

constexpr auto macSize = 6;
constexpr std::array<uint8_t, macSize> broadcastAddress{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
constexpr std::array<uint8_t, macSize> msgSignature{'T', 'H', 'D', 'T'};

void EspNow::init(NewReadingsCb newReadingsCb)
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

void EspNow::onDataRecv(const uint8_t *macAddr, const uint8_t *incomingData, int len)
{
    THMessage recvMsg{};
    memcpy(&recvMsg, incomingData, sizeof(recvMsg));
    switch (recvMsg.type)
    {
    case PAIR_REQ:
        logger::logInf("PAIR_REQ received");
        addPeer(macAddr, WiFi.channel());
        sendPairOK(macAddr);
        break;
    case PAIR_OK:
        logger::logErr("PAIR_OK received, but this message should be handled by sensor");
        break;
    case TH_DATA:
        logger::logInf("TH_DATA received");
        logger::logInfF("T: %f, H: %f", recvMsg.temperature, recvMsg.humidity);

        // recvCB(recvMsg.temperature, recvMsg.humidity, macToStrNoDots(recvMsg.mac));
        m_newReadingsCb(recvMsg.temperature, recvMsg.humidity, "");
        break;
    }
}

void EspNow::onDataSend(const uint8_t *macAddr, esp_now_send_status_t status)
{
    logger::logInf("Last Packet Send Status: ");
    if (status == 0)
    {
        // String mac = macToStr(macAddr);
        logger::logInf("Delivery success: ");
        // logger::logInf(mac);
    }
    else
    {
        logger::logErr("Delivery fail");
    }
}

void EspNow::setOnDataRecvCb()
{
    static auto onRecvDataThis = [this](const uint8_t *macAddr, const uint8_t *incomingData,
                                        int len) { this->onDataRecv(macAddr, incomingData, len); };
    auto onDataRecv = [](const uint8_t *macAddr, const uint8_t *incomingData, int len)
    { onRecvDataThis(macAddr, incomingData, len); };

    esp_now_register_recv_cb(onDataRecv);
}

void EspNow::setOnDataSendCb()
{
    static auto onDataSendThis = [this](const uint8_t *macAddr, esp_now_send_status_t status)
    { this->onDataSend(macAddr, status); };
    auto onDataSend = [](const uint8_t *macAddr, esp_now_send_status_t status)
    { onDataSendThis(macAddr, status); };

    esp_now_register_send_cb(onDataSend);
}

void EspNow::addPeer(const uint8_t *macAddr, uint8_t channel)
{
    esp_now_peer_info_t peer = {};
    memcpy(&peer.peer_addr[0], macAddr, ESP_NOW_ETH_ALEN);
    peer.channel = channel;
    esp_now_add_peer(&peer);
}

void EspNow::sendPairOK(const uint8_t *mac)
{
    THMessage msg{};
    msg.type = PAIR_OK;
    msg.channel = WiFi.channel();
    WiFi.softAPmacAddress(&msg.mac[0]);
    memcpy(&msg.signature[0], msgSignature.data(), sizeof(msgSignature));
    auto state = esp_now_send(mac, reinterpret_cast<uint8_t *>(&msg), sizeof(msg));  // NOLINT

    if (state != ESP_OK)
    {
        logger::logErrF("esp_now_send error, code: %d", state);
    }
}