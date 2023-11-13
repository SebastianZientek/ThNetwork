#pragma once

#include <Arduino.h>
#include <NTPClient.h>
#include <esp_now.h>

#include <functional>
#include <memory>

#include "MacAddr.hpp"
#include "Messages.hpp"

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
    void onDataRecv(MacAddr mac, const uint8_t *incomingData, int len);
    void onDataSend(MacAddr mac, esp_now_send_status_t status);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void addPeer(MacAddr mac, uint8_t channel);
    void sendPairOK(MacAddr mac) const;
    MsgType getMsgType(const uint8_t *buffer, size_t size);

    NewReadingsCb m_newReadingsCb;
    std::shared_ptr<NTPClient> m_ntpClient;
    uint8_t m_sensorUpdatePeriodMins;
};
