#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "EspAdp.hpp"
#include "EspNow.hpp"
#include "EspNowAdp.hpp"
#include "Messages.hpp"

TEST_GROUP(TestEspNow)  // NOLINT
{
    void setup() override
    {
    }

    void teardown() override
    {
        mock().clear();
        mock().checkExpectations();
    }

    void initEspNow(EspNow & espNow)
    {
        mock().expectOneCall("WiFiAdp::setModeSta");
        mock().expectOneCall("WiFiAdp::setChannel").withParameter("channel", 0);
        mock().expectOneCall("WiFiAdp::disconnect");

        mock().expectOneCall("EspNowAdp::init").andReturnValue(0);
        mock().expectOneCall("EspNowAdp::setComboRole");

        mock().expectOneCall("EspNowAdp::registerRecvCB");
        mock().expectOneCall("EspNowAdp::registerSendCB");

        espNow.init();
    }

    void acceptPairRespMsg(EspNow & espNow)
    {
        auto recvCb = EspNowAdp::getRecvCB();
        MacAddr mac = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};  // NOLINT

        auto msg = PairRespMsg::create();
        msg.channel = 1;
        msg.hostMacAddr = mac;
        msg.updatePeriodMins = 2;

        auto serializedMsg = msg.serialize();
        recvCb(mac.data(), serializedMsg.data(), serializedMsg.size());

        auto transmitterConfig = espNow.getTransmitterConfig();
        CHECK_EQUAL(msg.channel, transmitterConfig.channel);         // NOLINT
        CHECK_TRUE(msg.hostMacAddr == transmitterConfig.targetMac);  // NOLINT
    }

    constexpr static auto LED_BUILTIN = 9;
};

TEST(TestEspNow, ShouldInitEspNowWithCallbacks)  // NOLINT
{
    EspNow espNow;
    initEspNow(espNow);
}

TEST(TestEspNow, ShouldAcceptPairMessage)  // NOLINT
{
    EspNow espNow;
    initEspNow(espNow);
    acceptPairRespMsg(espNow);
}

TEST(TestEspNow, ShouldDoNothingOnNotSupportedMessages)  // NOLINT
{
    EspNow espNow;
    initEspNow(espNow);

    auto recvCb = EspNowAdp::getRecvCB();
    std::array<uint8_t, 6> mac = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};  // NOLINT

    auto msg1 = PairReqMsg::create();
    auto serializedMsg1 = msg1.serialize();
    recvCb(mac.data(), serializedMsg1.data(), serializedMsg1.size());

    auto msg2 = SensorDataMsg::create();
    auto serializedMsg2 = msg2.serialize();
    recvCb(mac.data(), serializedMsg2.data(), serializedMsg2.size());

    auto msg3 = SensorDataMsg::create();
    msg3.msgType = MsgType::UNKNOWN;
    auto serializedMsg3 = msg3.serialize();
    recvCb(mac.data(), serializedMsg3.data(), serializedMsg3.size());
}

TEST(TestEspNow, ShouldReturnNulloptIfCantPair)  // NOLINT
{
    EspNow espNow;
    initEspNow(espNow);
    constexpr auto maxChannelNum = 12;
    constexpr auto waitTime = 1000;

    for (int i = 0; i <= maxChannelNum; ++i)
    {
        mock().expectOneCall("WiFiAdp::setChannel").withParameter("channel", i);
        mock().expectOneCall("EspNowAdp::send").andReturnValue(0);
        mock().expectOneCall("millis").andReturnValue(0);
        mock().expectOneCall("millis").andReturnValue(waitTime);
    }

    CHECK_FALSE(espNow.pair());  // NOLINT
}

#include <iostream>

TEST(TestEspNow, ShouldSetupAfterPairMsg)  // NOLINT
{
    EspNow espNow;
    initEspNow(espNow);

    auto onWaitCb = EspAdp::createAndInjectOnWaitCb([this, &espNow] { acceptPairRespMsg(espNow); });
    constexpr auto waitTime = 1000;

    mock().expectOneCall("WiFiAdp::setChannel").withParameter("channel", 0);
    mock().expectOneCall("EspNowAdp::send").andReturnValue(0);
    mock().expectOneCall("millis").andReturnValue(0);
    mock().expectOneCall("millis").andReturnValue(waitTime);

    CHECK_TRUE(espNow.pair());  // NOLINT
}

TEST(TestEspNow, ShouldSendDataToHost)  // NOLINT
{
    EspNow espNow;

    mock().expectOneCall("EspNowAdp::send").andReturnValue(0);
    mock().expectOneCall("delay").withParameter("time", 1);

    MacAddr mac = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};  // NOLINT
    espNow.sendDataToHost(mac, 12.3, 4.56);              // NOLINT
}
