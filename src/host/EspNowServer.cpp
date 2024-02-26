#include "EspNowServer.hpp"

#include <array>

#include "common/Messages.hpp"
#include "common/logger.hpp"
#include "common/serializer.hpp"

constexpr auto macSize = 6;
constexpr auto msgSignatureSize = 4;
constexpr std::array<uint8_t, macSize> broadcastAddress{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

EspNowServer::EspNowServer(std::unique_ptr<IEspNow32Adp> espNowAdp,
                           std::shared_ptr<EspNowPairingManager> pairingManager,
                           std::shared_ptr<IWifi32Adp> wifiAdp)
    : m_espNowAdp(std::move(espNowAdp))
    , m_sensorUpdatePeriodMins(1)
    , m_pairingManager(pairingManager)
    , m_wifiAdp(wifiAdp)
{
}

void EspNowServer::init(const NewReadingsCb &newReadingsCb, uint8_t sensorUpdatePeriodMins)
{
    if (m_espNowAdp->init() == IEspNow32Adp::Status::FAIL)
    {
        logger::logErr("Can't init Esp Now");
        return;
    }

    m_newReadingsCb = newReadingsCb;
    m_sensorUpdatePeriodMins = sensorUpdatePeriodMins;
    setOnDataRecvCb();
    setOnDataSendCb();
}

void EspNowServer::deinit()
{
    m_espNowAdp->deinit();
}

void EspNowServer::onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len)
{
    auto msgAndSignature = serializer::partialDeserialize<MsgType, Signature>(incomingData, len);

    if (!msgAndSignature)
    {
        logger::logWrn("Can't deserialize received message");
        return;
    }

    auto [msgType, signature] = msgAndSignature.value();

    if (signature != signatureTemplate)
    {
        std::string sigStr(signature.begin(), signature.end());
        std::string templateSigStr(signatureTemplate.begin(), signatureTemplate.end());
        logger::logWrn("Received message with wrong signature");
        return;
    }

    switch (msgType)
    {
    case MsgType::PAIR_REQ:
    {
        if (m_pairingManager->isPairingEnabled())
        {
            logger::logDbg("PAIR_REQ received");
            PairReqMsg pairReqMsg;
            pairReqMsg.deserialize(incomingData, len);
            m_pairingManager->addNewSensorToStorage(pairReqMsg.ID);

            logger::logInf("Paired sensor: %u", pairReqMsg.ID);
            m_espNowAdp->addPeer(mac, m_wifiAdp->getChannel());
            sendPairOK(mac);
            m_espNowAdp->deletePeer(mac);
        }
        else
        {
            logger::logWrn("Pairing not enabled, request rejected");
        }
    }
    break;
    case MsgType::PAIR_RESP:
        logger::logWrn("Host shouldn't receive pair resp.");
        break;
    case MsgType::SENSOR_DATA:
    {
        SensorDataMsg sDataMsg;
        sDataMsg.deserialize(incomingData, len);

        if (m_pairingManager->isPaired(sDataMsg.ID))
        {
            m_newReadingsCb(sDataMsg.temperature, sDataMsg.humidity, sDataMsg.ID);
        }
        else
        {
            logger::logWrn("Ignored data from unpaired sensor, id: %u", sDataMsg.ID);
        }
    }
    break;
    case MsgType::UNKNOWN:
        logger::logWrn("Received UNKNOWN message type.");
    }
}

void EspNowServer::onDataSend(const MacAddr &mac, IEspNow32Adp::Status status)
{
    logger::logDbg("Last Packet Send Status: ");
    if (status == IEspNow32Adp::Status::OK)
    {
        logger::logDbg("Delivery success: %s", mac.str());
    }
    else
    {
        logger::logDbg("Delivery fail");
    }
}

void EspNowServer::setOnDataSendCb()
{
    auto onSend = [this](const MacAddr &mac, IEspNow32Adp::Status status)
    {
        this->onDataSend(mac, status);
    };
    m_espNowAdp->registerOnSendCb(onSend);
}

void EspNowServer::setOnDataRecvCb()
{
    auto onRecv = [this](MacAddr mac, const uint8_t *incomingData, int len)
    {
        this->onDataRecv(mac, incomingData, len);
    };

    m_espNowAdp->registerOnRecvCb(onRecv);
}

void EspNowServer::sendPairOK(const MacAddr &mac) const
{
    auto pairRespMsg = PairRespMsg::create(static_cast<uint8_t>(m_wifiAdp->getChannel()),
                                           m_sensorUpdatePeriodMins);
    pairRespMsg.hostMacAddr.addrData = m_wifiAdp->getSoftApMacAddr();
    auto buffer = pairRespMsg.serialize();

    auto state = m_espNowAdp->sendData(mac, buffer.data(), buffer.size());

    if (state == IEspNow32Adp::Status::FAIL)
    {
        logger::logWrn("Esp send msg error");
    }
}
