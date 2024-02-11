#pragma once

#include <Arduino.h>
#include <NTPClient.h>
#include <esp_now.h>

#include <functional>
#include <memory>

#include "EspNowPairingManager.hpp"
#include "common/MacAddr.hpp"
#include "common/Messages.hpp"
#include "common/types.hpp"

class EspNowServer
{
public:
    using NewReadingsCb
        = std::function<void(float temp, float hum, IDType identifier, unsigned long epochTime)>;
    using NewPeerCb = std::function<bool(IDType identifier)>;

    EspNowServer(std::shared_ptr<EspNowPairingManager> pairingManager,
           std::shared_ptr<NTPClient> ntpClient);
    ~EspNowServer() = default;
    EspNowServer(const EspNowServer &) = delete;
    EspNowServer(EspNowServer &&) = delete;
    EspNowServer &operator=(const EspNowServer &) = delete;
    EspNowServer &operator=(EspNowServer &&) = delete;

    void init(const NewReadingsCb &newReadingsCb,
              const NewPeerCb &newPeerCb,
              uint8_t sensorUpdatePeriodMins);
    void deinit();

private:
    using OnSendCb = std::function<void(const MacAddr &mac, esp_now_send_status_t status)>;
    using OnRecvCb
        = std::function<void(const MacAddr &mac, const uint8_t *incomingData, uint8_t len)>;

    static OnSendCb m_onSend;  // NOLINT
    static OnRecvCb m_onRecv;  // NOLINT

    NewReadingsCb m_newReadingsCb;
    NewPeerCb m_newPeerCb;
    std::shared_ptr<NTPClient> m_ntpClient;
    std::shared_ptr<EspNowPairingManager> m_pairingManager;
    uint8_t m_sensorUpdatePeriodMins;
    bool m_pairingEnabled = false;

    void onDataSend(const MacAddr &mac, esp_now_send_status_t status);
    void onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void addPeer(const MacAddr &mac, uint8_t channel);
    void sendPairOK(const MacAddr &mac) const;
};
