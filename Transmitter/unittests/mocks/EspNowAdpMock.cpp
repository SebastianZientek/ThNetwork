#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "EspNowAdp.hpp"

int EspNowAdp::init()
{
    return mock().actualCall("EspNowAdp::init").returnIntValue();
}

void EspNowAdp::setComboRole()
{
    mock().actualCall("EspNowAdp::setComboRole");
}

int EspNowAdp::registerRecvCB(RecvCB callback)
{
    m_recvCb = callback;
    return mock().actualCall("EspNowAdp::registerRecvCB").returnIntValue();
}

int EspNowAdp::registerSendCB(SendCB callback)
{
    m_sendCb = callback;
    return mock().actualCall("EspNowAdp::registerSendCB").returnIntValue();
}

int EspNowAdp::send(uint8_t *addr, uint8_t *data, int len)
{
    return mock().actualCall("EspNowAdp::send").returnIntValue();
}

EspNowAdp::RecvCB EspNowAdp::m_recvCb{};  // NOLINT
EspNowAdp::SendCB EspNowAdp::m_sendCb{};  // NOLINT

EspNowAdp::RecvCB EspNowAdp::getRecvCB()
{
    return m_recvCb;
}

EspNowAdp::SendCB EspNowAdp::getSendCB()
{
    return m_sendCb;
}
