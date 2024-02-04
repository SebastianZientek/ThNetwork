#pragma once

#include <Arduino.h>
#include <NTPClient.h>
#include <esp_now.h>

#include <functional>
#include <memory>

#include "common/MacAddr.hpp"
#include "common/Messages.hpp"
#include "common/types.hpp"

class EspNow
{
public:
    using NewReadingsCb
        = std::function<void(float temp, float hum, IDType identifier, unsigned long epochTime)>;
    using NewPeerCb = std::function<void(IDType identifier)>;

    EspNow(std::shared_ptr<NTPClient> ntpClient);
    ~EspNow() = default;
    EspNow(const EspNow &) = delete;
    EspNow(EspNow &&) = delete;
    EspNow &operator=(const EspNow &) = delete;
    EspNow &operator=(EspNow &&) = delete;

    void init(const NewReadingsCb &newReadingsCb,
              const NewPeerCb &newPeerCb,
              uint8_t sensorUpdatePeriodMins);
    void enablePairing();
    void disablePairing();
    void deinit();

private:
    using OnSendCb = std::function<void(const MacAddr &mac, esp_now_send_status_t status)>;
    using OnRecvCb
        = std::function<void(const MacAddr &mac, const uint8_t *incomingData, uint8_t len)>;

    static OnSendCb m_onSend;  // NOLINT
    static OnRecvCb m_onRecv;  // NOLINT

    constexpr static std::size_t m_pairingTimeout = 3;// * 60 * 1000; // 3 minutes
    std::size_t m_pairingStartTime = 0;
    bool m_pairingEnabled = false;

    NewReadingsCb m_newReadingsCb;
    NewPeerCb m_newPeerCb;
    std::shared_ptr<NTPClient> m_ntpClient;
    uint8_t m_sensorUpdatePeriodMins;

    void onDataSend(const MacAddr &mac, esp_now_send_status_t status);
    void onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void addPeer(const MacAddr &mac, uint8_t channel);
    void sendPairOK(const MacAddr &mac) const;
};
