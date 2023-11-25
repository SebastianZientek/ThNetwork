#include "EspNowAdp.hpp"

#include <ESP8266WiFi.h>
#include <espnow.h>

int EspNowAdp::init()
{
    return esp_now_init();
}

void EspNowAdp::setComboRole()
{
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
}

int EspNowAdp::registerRecvCB(RecvCB callback)
{
    return esp_now_register_recv_cb(callback);
}

int EspNowAdp::registerSendCB(SendCB callback)
{
    return esp_now_register_send_cb(callback);
}

int EspNowAdp::send(uint8_t *addr, uint8_t *data, int len) // NOLINT
{
    return esp_now_send(addr, data, len);
}
