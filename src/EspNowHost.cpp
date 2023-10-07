#include "EspNowHost.hpp"

#include <Arduino.h>

#include "esp_now.h"

namespace
{
RecvCB recvCB;

String macToStr(const uint8_t *macAddr)
{
    String addr;

    char macCStr[18];
    snprintf(macCStr, sizeof(macCStr), "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1],
             macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    addr = macCStr;
    return addr;
}

THMessage m;
uint8_t broadcastAddressX[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peer;
void espNowDataReceived(const uint8_t *macAddr, const uint8_t *incomingData, int len)
{
    Serial.println("Received!");
    THMessage message;
    memcpy(&message, incomingData, sizeof(message));

    switch (message.type)
    {
    case PAIR_REQ:
        Serial.println("PAIR_REQ");
        m.type = PAIR_OK;
        delay(100);
        Serial.println(ESP_ERR_ESPNOW_NOT_INIT);
        Serial.println(ESP_ERR_ESPNOW_ARG);
        Serial.println(ESP_ERR_ESPNOW_INTERNAL);
        Serial.println(ESP_ERR_ESPNOW_NO_MEM);
        Serial.println(ESP_ERR_ESPNOW_NOT_FOUND);
        Serial.println(ESP_ERR_ESPNOW_IF);

        memcpy(peer.peer_addr, macAddr, ESP_NOW_ETH_ALEN);
        peer.channel = 11;
        esp_now_add_peer(&peer);
        Serial.println(esp_now_send(macAddr, (uint8_t *)&m, sizeof(m)));
        delay(500);
        // esp_now_send(NULL, (uint8_t *) &outgoingSetpoints, sizeof(outgoingSetpoints));
        break;
    case PAIR_OK:
        Serial.println("PAIR_OK");
        break;
    case PAIR_NOK:
        Serial.println("PAIR_NOK");
        break;
    case TH_DATA:
        Serial.println("TH_DATA");
        break;
    }
    String addr = macToStr(macAddr);

    recvCB(message, addr);
}

void espNowDataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("Last Packet Send Status: ");
    if (status == 0)
    {
        String mac = macToStr(mac_addr);
        Serial.print("Delivery success: ");
        Serial.println(mac);
    }
    else
    {
        Serial.println("Delivery fail");
    }
}

}  // namespace

void initEspNow(RecvCB cb)
{
    recvCB = cb;
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error init ESP NOW");
        return;
    }

    esp_now_register_send_cb(espNowDataSend);
    esp_now_register_recv_cb(espNowDataReceived);
}
