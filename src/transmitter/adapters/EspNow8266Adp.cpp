#include "EspNow8266Adp.hpp"

#include <ESP8266WiFi.h>
#include <espnow.h>

EspNow8266Adp::OnSendCb EspNow8266Adp::m_onSendCb;  // NOLINT
EspNow8266Adp::OnRecvCb EspNow8266Adp::m_onRecvCb;  // NOLINT

EspNow8266Adp::Status EspNow8266Adp::init()
{
    if (esp_now_init() != 0)
    {
        return EspNow8266Adp::Status::FAIL;
    }
    return EspNow8266Adp::Status::OK;
}

void EspNow8266Adp::deinit()
{
    esp_now_deinit();
}

void EspNow8266Adp::setRole(Role role)
{
    switch (role)
    {
    case Role::IDLE:
        esp_now_set_self_role(ESP_NOW_ROLE_IDLE);
        break;
    case Role::CONTROLLER:
        esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
        break;
    case Role::SLAVE:
        esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
        break;
    case Role::COMBO:
        esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
        break;
    case Role::MAX:
        esp_now_set_self_role(ESP_NOW_ROLE_MAX);
        break;
    }
}

void EspNow8266Adp::registerOnSendCb(const OnSendCb &onSendCb)
{
    m_onSendCb = onSendCb;
    auto onDataSend = [](uint8_t *rawMac, uint8_t status)
    {
        MacAddr macAddr{};
        std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);

        m_onSendCb(macAddr, status == 0 ? Status::OK : Status::FAIL);
    };
    esp_now_register_send_cb(onDataSend);
}

void EspNow8266Adp::registerOnRecvCb(const OnRecvCb &onRecvCb)
{
    m_onRecvCb = onRecvCb;
    auto onDataRecv = [](uint8_t *rawMac, uint8_t *incomingData, uint8_t len)
    {
        MacAddr macAddr{};
        std::memcpy(macAddr.data(), rawMac, MacAddr::macAddrDigits);

        m_onRecvCb(macAddr, incomingData, len);
    };

    esp_now_register_recv_cb(onDataRecv);
}

EspNow8266Adp::Status EspNow8266Adp::sendData(MacAddr &mac, uint8_t *data, size_t length)
{
    auto state = esp_now_send(mac.data(), data, length);
    return state == 0 ? Status::OK : Status::FAIL;
}
