#pragma once

#include <Arduino.h>
#include <esp_now.h>

#include <functional>

class EspNow
{
public:
    using NewReadingsCb = std::function<void(float temp, float hum, String mac)>;

    EspNow()
    {
    }
    ~EspNow()
    {
    }

    EspNow(const EspNow &) = delete;
    EspNow(EspNow &&) = delete;
    EspNow &operator=(const EspNow &) = delete;
    EspNow &operator=(EspNow &&) = delete;

    void init(NewReadingsCb newReadingsCb);

private:
    void onDataRecv(const uint8_t *macAddr, const uint8_t *incomingData, int len);
    void onDataSend(const uint8_t *macAddr, esp_now_send_status_t status);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void addPeer(const uint8_t *macAddr, uint8_t channel);
    void sendPairOK(const uint8_t *mac);

    NewReadingsCb m_newReadingsCb;
};