#pragma once

#include <functional>
#include <memory>

#include "EspNowPairingManager.hpp"
#include "adapters/IEspNow32Adp.hpp"
#include "adapters/IWifi32Adp.hpp"
#include "common/MacAddr.hpp"
#include "common/Messages.hpp"
#include "common/types.hpp"

class EspNowServer
{
public:
    using NewReadingsCb = std::function<void(float temp, float hum, IDType identifier)>;
    using NewPeerCb = std::function<bool(IDType identifier)>;
    using SensorUpdateCb = std::function<void(uint8_t sensorUpdatePeriodMins)>;

    EspNowServer(std::unique_ptr<IEspNow32Adp> espNowAdp,
                 std::shared_ptr<EspNowPairingManager> pairingManager,
                 std::shared_ptr<IWifi32Adp> wifiAdp,
                 std::shared_ptr<IConfStorage> confStorage);
    ~EspNowServer() = default;
    EspNowServer(const EspNowServer &) = delete;
    EspNowServer(EspNowServer &&) = delete;
    EspNowServer &operator=(const EspNowServer &) = delete;
    EspNowServer &operator=(EspNowServer &&) = delete;

    void init(const NewReadingsCb &newReadingsCb);
    void deinit();

private:
    NewReadingsCb m_newReadingsCb;

    std::unique_ptr<IEspNow32Adp> m_espNowAdp;
    std::shared_ptr<EspNowPairingManager> m_pairingManager;
    std::shared_ptr<IWifi32Adp> m_wifiAdp;
    std::shared_ptr<IConfStorage> m_confStorage;
    bool m_pairingEnabled = false;

    void onDataSend(const MacAddr &mac, IEspNow32Adp::Status status);
    void onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void sendPairOK(const MacAddr &mac) const;
};
