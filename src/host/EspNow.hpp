#pragma once

#include <Arduino.h>
#include <NTPClient.h>
#include <esp_now.h>

#include <functional>
#include <memory>

#include "common/MacAddr.hpp"
#include "common/Messages.hpp"

class EspNow
{
public:
    using NewReadingsCb
        = std::function<void(float temp, float hum, MacAddr mac, unsigned long epochTime)>;

    EspNow(std::shared_ptr<NTPClient> ntpClient);
    ~EspNow() = default;
    EspNow(const EspNow &) = delete;
    EspNow(EspNow &&) = delete;
    EspNow &operator=(const EspNow &) = delete;
    EspNow &operator=(EspNow &&) = delete;

    void init(const NewReadingsCb &newReadingsCb, uint8_t sensorUpdatePeriodMins);

private:
    using OnSendCb = std::function<void(const MacAddr &mac, esp_now_send_status_t status)>;
    using OnRecvCb
        = std::function<void(const MacAddr &mac, const uint8_t *incomingData, uint8_t len)>;

    static OnSendCb m_onSend;  // NOLINT
    static OnRecvCb m_onRecv;  // NOLINT

    NewReadingsCb m_newReadingsCb;
    std::shared_ptr<NTPClient> m_ntpClient;
    uint8_t m_sensorUpdatePeriodMins;

    void onDataSend(const MacAddr &mac, esp_now_send_status_t status);
    void onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void addPeer(const MacAddr &mac, uint8_t channel);
    void sendPairOK(const MacAddr &mac) const;
    MsgType getMsgType(const uint8_t *buffer, size_t size);
};
