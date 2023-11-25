#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "WiFiAdp.hpp"

void WiFiAdp::setModeSta()
{
    mock().actualCall("WiFiAdp::setModeSta").returnIntValue();
}

void WiFiAdp::setChannel(uint8_t channel)
{
    mock().actualCall("WiFiAdp::setChannel").withParameter("channel", channel);
}

void WiFiAdp::disconnect()
{
    mock().actualCall("WiFiAdp::disconnect");
}

uint8_t *WiFiAdp::macAddress(uint8_t *mac)
{
    // mock().actualCall("WiFiAdp::macAddress").withParameter("mac", mac);
    return nullptr;
}
