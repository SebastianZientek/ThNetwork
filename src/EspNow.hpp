#pragma once

#include <Arduino.h>
#include <NTPClient.h>
#include <esp_now.h>

#include <functional>

#include "MacAddr.hpp"

class EspNow
{
public:
    using NewReadingsCb = std::function<void(float temp, float hum, String mac, unsigned long epochTime)>;

    EspNow(NTPClient &ntpClient);
    ~EspNow() = default;
    EspNow(const EspNow &) = delete;
    EspNow(EspNow &&) = delete;
    EspNow &operator=(const EspNow &) = delete;
    EspNow &operator=(EspNow &&) = delete;

    void init(const NewReadingsCb &newReadingsCb);

private:
    void onDataRecv(MacAddr mac, const uint8_t *incomingData, int len);
    void onDataSend(MacAddr mac, esp_now_send_status_t status);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void addPeer(MacAddr mac, uint8_t channel);
    void sendPairOK(MacAddr mac);

    NewReadingsCb m_newReadingsCb;
    NTPClient &m_ntpClient;
};
