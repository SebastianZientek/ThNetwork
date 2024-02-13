#pragma once

#include <cstdint>

struct EspNowAdp
{
    using RecvCB = void (*)(uint8_t *mac_addr, uint8_t *data, uint8_t len);
    using SendCB = void (*)(uint8_t *mac_addr, uint8_t status);

    static int init();
    static void deinit();
    static void setComboRole();
    static int registerRecvCB(RecvCB callback);
    static int registerSendCB(SendCB callback);
    static int send(uint8_t *addr, uint8_t *data, int len);
#ifdef UNIT_TESTS

    static RecvCB getRecvCB();
    static SendCB getSendCB();

private:
    static SendCB m_sendCb;  // NOLINT
    static RecvCB m_recvCb;  // NOLINT
#endif
};
