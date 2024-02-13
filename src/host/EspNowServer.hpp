#pragma once

#include <Arduino.h>
#include <esp_now.h>

#include <functional>
#include <memory>

#include "EspNowPairingManager.hpp"
#include "adapters/esp32/IEspNow32Adp.hpp"
#include "adapters/esp32/IWifi32Adp.hpp"
#include "common/MacAddr.hpp"
#include "common/Messages.hpp"
#include "common/types.hpp"

class EspNowServer
{
public:
    using NewReadingsCb = std::function<void(float temp, float hum, IDType identifier)>;
    using NewPeerCb = std::function<bool(IDType identifier)>;

    EspNowServer(std::unique_ptr<IEspNow32Adp> espNowAdp,
                 std::shared_ptr<EspNowPairingManager> pairingManager,
                 std::shared_ptr<IWifi32Adp> wifiAdp);
    ~EspNowServer() = default;
    EspNowServer(const EspNowServer &) = delete;
    EspNowServer(EspNowServer &&) = delete;
    EspNowServer &operator=(const EspNowServer &) = delete;
    EspNowServer &operator=(EspNowServer &&) = delete;

    void init(const NewReadingsCb &newReadingsCb, uint8_t sensorUpdatePeriodMins);
    void deinit();

private:
    NewReadingsCb m_newReadingsCb;

    std::unique_ptr<IEspNow32Adp> m_espNowAdp;
    std::shared_ptr<EspNowPairingManager> m_pairingManager;
    std::shared_ptr<IWifi32Adp> m_wifiAdp;
    uint8_t m_sensorUpdatePeriodMins;
    bool m_pairingEnabled = false;

    void onDataSend(const MacAddr &mac, IEspNow32Adp::Status status);
    void onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void sendPairOK(const MacAddr &mac) const;
};
