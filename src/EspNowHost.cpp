#include "EspNowHost.hpp"

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "logger.hpp"

namespace
{
RecvCB recvCB;                                                       // NOLINT
uint8_t broadcastAddressX[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // NOLINT
uint8_t msgSignature[4] = {'T', 'H', 'D', 'T'};                      // NOLINT

String macToStr(const uint8_t *macAddr)
{
    String addr;

    char macCStr[18];                                                                // NOLINT
    snprintf(macCStr, sizeof(macCStr), "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0],  // NOLINT
             macAddr[1],                                                             // NOLINT
             macAddr[2], macAddr[3], macAddr[4], macAddr[5]);                        // NOLINT
    addr = macCStr;                                                                  // NOLINT
    return addr;
}

void addPeer(const uint8_t *macAddr, uint8_t channel)
{
    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, macAddr, ESP_NOW_ETH_ALEN);  // NOLINT
    peer.channel = channel;
    esp_now_add_peer(&peer);
}

void sendPairOK(const uint8_t *mac)
{
    THMessage msg{};
    msg.type = PAIR_OK;
    msg.channel = WiFi.channel();
    WiFi.softAPmacAddress(msg.mac);                                // NOLINT
    memcpy(msg.signature, msgSignature, sizeof(msgSignature));     // NOLINT
    auto state = esp_now_send(mac, (uint8_t *)&msg, sizeof(msg));  // NOLINT

    if (state != ESP_OK)
    {
        logger::logErrF("esp_now_send error, code: %d", state);
    }
}

void onDataRecv(const uint8_t *macAddr, const uint8_t *incomingData, int len)
{
    THMessage recvMsg{};
    memcpy(&recvMsg, incomingData, sizeof(recvMsg));
    switch (recvMsg.type)
    {
    case PAIR_REQ:
        logger::logInf("PAIR_REQ received");
        addPeer(macAddr, WiFi.channel());
        sendPairOK(macAddr);
        break;
    case PAIR_OK:
        logger::logErr("PAIR_OK received, but this message should be handled by sensor");
        break;
    case TH_DATA:
        logger::logInf("TH_DATA received");
        logger::logInfF("T: %f, H: %f", recvMsg.temperature, recvMsg.humidity);
        recvCB(recvMsg.temperature, recvMsg.humidity, macToStr(recvMsg.mac));  // NOLINT
        break;
    }
    // String addr = macToStr(macAddr);

    // recvCB(message, addr);
}

void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    logger::logInf("Last Packet Send Status: ");
    if (status == 0)
    {
        String mac = macToStr(mac_addr);
        logger::logInf("Delivery success: ");
        logger::logInf(mac);
    }
    else
    {
        logger::logErr("Delivery fail");
    }
}

}  // namespace

void initEspNow(RecvCB cb)
{
    recvCB = cb;
    if (esp_now_init() != ESP_OK)
    {
        logger::logErr("Init ESP NOW issue");
        return;
    }

    esp_now_register_send_cb(onDataSend);
    esp_now_register_recv_cb(onDataRecv);
}
