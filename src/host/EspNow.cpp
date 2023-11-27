#include "EspNow.hpp"

#include <WiFi.h>

#include <array>

#include "common/Messages.hpp"
#include "NTPClient.h"
#include "common/logger.hpp"

constexpr auto macSize = 6;
constexpr auto msgSignatureSize = 4;
constexpr std::array<uint8_t, macSize> broadcastAddress{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
constexpr std::array<uint8_t, 4> msgSignature{'T', 'H', 'D', 'T'};
EspNow::OnSendCb EspNow::m_onSend;  // NOLINT
EspNow::OnRecvCb EspNow::m_onRecv;  // NOLINT

EspNow::EspNow(std::shared_ptr<NTPClient> ntpClient)
    : m_ntpClient(ntpClient)
    , m_sensorUpdatePeriodMins(1)
{
}

void EspNow::init(const NewReadingsCb &newReadingsCb, uint8_t sensorUpdatePeriodMins)
{
    if (esp_now_init() != ESP_OK)
    {
        logger::logErr("Init ESP NOW issue");
        return;
    }

    m_newReadingsCb = newReadingsCb;
    m_sensorUpdatePeriodMins = sensorUpdatePeriodMins;
    setOnDataRecvCb();
    setOnDataSendCb();
}

void EspNow::onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len)
{
    auto msgType = getMsgType(incomingData, len);

    switch (msgType)
    {
    case MsgType::PAIR_REQ:
    {
        logger::logInf("PAIR_REQ received");
        addPeer(mac, WiFi.channel());
        sendPairOK(mac);
    }
    break;
    case MsgType::PAIR_RESP:
        logger::logErr("Received PAIR_RESP, shouldn't be here.");
        break;
    case MsgType::SENSOR_DATA:
    {
        SensorDataMsg sDataMsg;
        sDataMsg.deserialize(incomingData, len);

        logger::logInf("[%s %s] T: %.1f, H: %.1f", mac.str().c_str(),
                        m_ntpClient->getFormattedTime().c_str(), sDataMsg.temperature,
                        sDataMsg.humidity);

        m_newReadingsCb(sDataMsg.temperature, sDataMsg.humidity, mac, m_ntpClient->getEpochTime());
    }
    break;
    case MsgType::UNKNOWN:
        logger::logErr("Received UNKNOWN message type.");
    }
}

void EspNow::onDataSend(const MacAddr &mac, esp_now_send_status_t status)
{
    logger::logInf("Last Packet Send Status: ");
    if (status == 0)
    {
        logger::logInf("Delivery success: %s", mac.str().c_str());
    }
    else
    {
        logger::logErr("Delivery fail");
    }
}

void EspNow::setOnDataSendCb()
{
    m_onSend = [this](const MacAddr &mac, esp_now_send_status_t status)
    { this->onDataSend(mac, status); };
    auto onDataSend = [](const uint8_t *rawMac, esp_now_send_status_t status)
    {
        MacAddr macAddr{};
        std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);

        m_onSend(macAddr, status);
    };

    esp_now_register_send_cb(onDataSend);
}

void EspNow::setOnDataRecvCb()
{
    m_onRecv = [this](MacAddr mac, const uint8_t *incomingData, int len)
    { this->onDataRecv(mac, incomingData, len); };
    auto onDataRecv = [](const uint8_t *rawMac, const uint8_t *incomingData, int len)
    {
        MacAddr macAddr{};
        std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);

        m_onRecv(macAddr, incomingData, len);
    };

    esp_now_register_recv_cb(onDataRecv);
}

void EspNow::addPeer(const MacAddr &mac, uint8_t channel)
{
    esp_now_peer_info_t peer = {};
    memcpy(&peer.peer_addr[0], mac.data(), ESP_NOW_ETH_ALEN);
    peer.channel = channel;
    esp_now_add_peer(&peer);
}

void EspNow::sendPairOK(const MacAddr &mac) const
{
    auto pairRespMsg
        = PairRespMsg::create(static_cast<uint8_t>(WiFi.channel()), m_sensorUpdatePeriodMins);
    WiFi.softAPmacAddress(pairRespMsg.hostMacAddr.data());
    auto buffer = pairRespMsg.serialize();

    auto state = esp_now_send(mac.data(), buffer.data(), buffer.size());

    if (state != ESP_OK)
    {
        logger::logErr("esp_now_send error, code: %d", state);
    }
}

MsgType EspNow::getMsgType(const uint8_t *buffer, size_t size)
{
    MsgType msgType{MsgType::UNKNOWN};

    if (size >= sizeof(MsgType))
    {
        std::memcpy(&msgType, buffer, sizeof(MsgType));
    }

    return msgType;
}
