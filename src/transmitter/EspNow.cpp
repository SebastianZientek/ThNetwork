#include "EspNow.hpp"

#include <optional>

#include "adapters/EspAdp.hpp"
#include "EspNowAdp.hpp"
#include "common/MacAddr.hpp"
#include "common/Messages.hpp"
#include "WiFiAdp.hpp"
#include "common/logger.hpp"
#include "utils.hpp"

EspNow::OnSendCb EspNow::m_onSend;  // NOLINT
EspNow::OnRecvCb EspNow::m_onRecv;  // NOLINT

void EspNow::init(uint8_t channel)
{
    WiFiAdp::setModeSta();
    WiFiAdp::setChannel(channel);
    WiFiAdp::disconnect();

    if (EspNowAdp::init() != 0)
    {
        logger::logInf("Error initializing ESP-NOW");
        return;
    }

    EspNowAdp::setComboRole();

    setOnDataRecvCb();
    setOnDataSendCb();
}

void EspNow::onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len)
{
    auto msgType = getMsgType(incomingData, len);

    switch (msgType)
    {
    case MsgType::PAIR_REQ:
        logger::logErr("PAIR_REQ is unsupported by transmitter");
        break;
    case MsgType::PAIR_RESP:
    {
        PairRespMsg pairRespMsg;
        pairRespMsg.deserialize(incomingData, len);

        m_paired = true;
        m_transmitterConfig.sensorUpdatePeriodMins = pairRespMsg.updatePeriodMins;
        m_transmitterConfig.channel = pairRespMsg.channel;
        m_transmitterConfig.targetMac = pairRespMsg.hostMacAddr;

        logger::logInf("Paired %s, ch: %d\n", pairRespMsg.hostMacAddr.str().c_str(),
                        pairRespMsg.channel);
    }
    break;
    case MsgType::SENSOR_DATA:
        logger::logErr("SENSOR_DATA is unsupported by transmitter");
        break;
    case MsgType::UNKNOWN:
        [[fallthrough]];
    default:
        logger::logErr("Wrong message type");
    }
}

void EspNow::onDataSend(const MacAddr &mac, uint8_t status)
{
    if (status == 0)
    {
        logger::logInf("Delivery success: %s", mac.str().c_str());
    }
    else
    {
        logger::logErr("Delivery fail");
    }
}

void EspNow::setOnDataRecvCb()
{
    m_onRecv = [this](const MacAddr &mac, const uint8_t *incomingData, uint8_t len)
    { this->onDataRecv(mac, incomingData, len); };
    auto onDataRecv = [](uint8_t *rawMac, uint8_t *incomingData, uint8_t len)  // NOLINT
    {
        MacAddr macAddr{};
        std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);
        m_onRecv(macAddr, incomingData, len);
    };

    EspNowAdp::registerRecvCB(onDataRecv);
}

void EspNow::setOnDataSendCb()
{
    m_onSend = [this](const MacAddr &mac, uint8_t status) { this->onDataSend(mac, status); };

    auto onDataSend = [](uint8_t *rawMac, uint8_t status)
    {
        MacAddr macAddr{};
        std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);
        m_onSend(macAddr, status);
    };

    EspNowAdp::registerSendCB(onDataSend);
}

std::optional<config::TransmitterConfig> EspNow::pair()
{
    constexpr auto maxChannels = 12;
    constexpr auto timeout = 200;
    m_paired = false;

    for (int channel = 0; channel <= maxChannels; ++channel)
    {
        logger::logInf("Pairing, try channel: %d", channel);
        WiFiAdp::setChannel(channel);
        sendPairMsg();
        EspAdp::wait(timeout);

        if (m_paired)
        {
            return m_transmitterConfig;
        }
    }

    return std::nullopt;
}

void EspNow::sendDataToHost(MacAddr mac, float temperature, float humidity)
{
    logger::logInf("Send data to %s", mac.str().c_str());

    auto sDataMsg = SensorDataMsg::create(temperature, humidity);
    WiFiAdp::macAddress(sDataMsg.transmitterMacAddr.data());
    auto buffer = sDataMsg.serialize();

    if (auto errCode = EspNowAdp::send(mac.data(), buffer.data(), buffer.size()); errCode)
    {
        logger::logErr("esp_now_send error, code: %d", errCode);
    }
    delay(1);  // Give board time to invoke onDataSent callback
}

config::TransmitterConfig EspNow::getTransmitterConfig()
{
    return m_transmitterConfig;
}

void EspNow::sendPairMsg()
{
    auto pairReqMsg = PairReqMsg::create();
    WiFiAdp::macAddress(pairReqMsg.transmitterMacAddr.data());
    auto buffer = pairReqMsg.serialize();

    std::array<uint8_t, 6> broadcastAddr{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // NOLINT
    if (auto errCode = EspNowAdp::send(broadcastAddr.data(), buffer.data(), buffer.size()); errCode)
    {
        logger::logErr("esp_now_send error, code: %d", errCode);
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
