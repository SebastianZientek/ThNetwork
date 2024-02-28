#include "EspNow.hpp"

#include <optional>

#include "common/MacAddr.hpp"
#include "common/Messages.hpp"
#include "common/logger.hpp"
#include "transmitter/adapters/EspNow8266Adp.hpp"
#include "utils.hpp"

EspNow::EspNow(std::shared_ptr<IArduino8266Adp> arduinoAdp,
               std::shared_ptr<IWifi8266Adp> wifiAdp,
               std::shared_ptr<IEsp8266Adp> espAdp,
               std::shared_ptr<IEspNow8266Adp> espNowAdp)
    : m_arduinoAdp(arduinoAdp)
    , m_wifiAdp(wifiAdp)
    , m_espAdp(espAdp)
    , m_espNowAdp(espNowAdp)
{
}

void EspNow::init(uint8_t channel)
{
    m_wifiAdp->setModeSta();
    m_wifiAdp->setChannel(channel);
    m_wifiAdp->disconnect();

    if (m_espNowAdp->init() != IEspNow8266Adp::Status::OK)
    {
        logger::logErr("Can't initialize esp-now");
        return;
    }

    m_espNowAdp->setRole(IEspNow8266Adp::Role::COMBO);

    setOnDataRecvCb();
    setOnDataSendCb();
}

IEspNow8266Adp::MsgHandleStatus EspNow::onDataRecv(const MacAddr &mac,
                                                   const uint8_t *incomingData,
                                                   int len)
{
    logger::logDbg("Received message");
    auto msgAndSignature = serializer::partialDeserialize<MsgType, Signature>(incomingData, len);

    if (!msgAndSignature)
    {
        logger::logWrn("Can't deserialize received message");
        return IEspNow8266Adp::MsgHandleStatus::REJECTED;
    }

    auto [msgType, signature] = msgAndSignature.value();

    if (signature != signatureTemplate)
    {
        logger::logWrn("Received message with wrong signature");
        return IEspNow8266Adp::MsgHandleStatus::REJECTED;
    }

    switch (msgType)
    {
    case MsgType::PAIR_REQ:
        logger::logWrn("PAIR_REQ is unsupported by transmitter");
        return IEspNow8266Adp::MsgHandleStatus::REJECTED;
        break;
    case MsgType::PAIR_RESP:
    {
        PairRespMsg pairRespMsg{};
        pairRespMsg.deserialize(incomingData, len);

        m_paired = true;

        auto myMac = MacAddr{};
        m_wifiAdp->macAddress(myMac.data());

        m_transmitterConfig.sensorUpdatePeriodMins = pairRespMsg.updatePeriodMins;
        m_transmitterConfig.channel = pairRespMsg.channel;
        m_transmitterConfig.targetMac = pairRespMsg.hostMacAddr;
        m_transmitterConfig.ID = myMac.toUniqueID();

        logger::logInf("Paired %s, ch: %d\n", pairRespMsg.hostMacAddr.str(), pairRespMsg.channel);
    }
    break;
    case MsgType::SENSOR_DATA:
        logger::logWrn("SENSOR_DATA is unsupported by transmitter");
        return IEspNow8266Adp::MsgHandleStatus::REJECTED;
        break;
    case MsgType::UNKNOWN:
        [[fallthrough]];
    default:
        logger::logWrn("Wrong message type");
        return IEspNow8266Adp::MsgHandleStatus::REJECTED;
    }

    return IEspNow8266Adp::MsgHandleStatus::ACCEPTED;
}

void EspNow::onDataSend(const MacAddr &mac, IEspNow8266Adp::Status status)
{
    if (status == IEspNow8266Adp::Status::OK)
    {
        logger::logDbg("Delivery success: %s", mac.str());
    }
    else
    {
        logger::logDbg("Delivery fail");
    }
}

void EspNow::setOnDataRecvCb()
{
    auto onRecv = [this](MacAddr mac, const uint8_t *incomingData, int len)
    {
        return this->onDataRecv(mac, incomingData, len);
    };

    m_espNowAdp->registerOnRecvCb(onRecv);
}

void EspNow::setOnDataSendCb()
{
    auto onSend = [this](const MacAddr &mac, IEspNow8266Adp::Status status)
    {
        this->onDataSend(mac, status);
    };

    m_espNowAdp->registerOnSendCb(onSend);
}

std::optional<config::TransmitterConfig> EspNow::pair()
{
    utils::switchOnLed(m_arduinoAdp);
    constexpr auto maxChannels = 12;
    constexpr auto timeout = 500;
    m_paired = false;

    for (int channel = 0; channel <= maxChannels; ++channel)
    {
        logger::logDbg("Pairing, try channel: %d", channel);
        m_wifiAdp->setChannel(channel);
        sendPairMsg();
        m_arduinoAdp->delay(timeout);
        m_espAdp->feedWatchdog();

        if (m_paired)
        {
            return m_transmitterConfig;
        }
    }
    utils::switchOffLed(m_arduinoAdp);

    return std::nullopt;
}

void EspNow::sendDataToHost(std::size_t identifier, MacAddr mac, float temperature, float humidity)
{
    logger::logDbg("Send data to %s", mac.str());

    auto sDataMsg = SensorDataMsg::create(identifier, temperature, humidity);
    auto buffer = sDataMsg.serialize();

    if (m_espNowAdp->sendData(mac, buffer.data(), buffer.size()) == IEspNow8266Adp::Status::FAIL)
    {
        logger::logWrn("EspNowAdp message send error");
    }
    m_arduinoAdp->delay(1);  // Give board time to invoke onDataSent callback
}

config::TransmitterConfig EspNow::getTransmitterConfig()
{
    return m_transmitterConfig;
}

void EspNow::sendPairMsg()
{
    auto pairReqMsg = PairReqMsg::create();
    m_wifiAdp->macAddress(pairReqMsg.transmitterMacAddr.data());
    pairReqMsg.ID = pairReqMsg.transmitterMacAddr.toUniqueID();
    auto buffer = pairReqMsg.serialize();

    MacAddr broadcastAddr{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // NOLINT
    if (m_espNowAdp->sendData(broadcastAddr, buffer.data(), buffer.size())
        == IEspNow8266Adp::Status::FAIL)
    {
        logger::logWrn("EspNowAdp send message error");
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
