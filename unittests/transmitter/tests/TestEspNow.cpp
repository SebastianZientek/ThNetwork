#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <memory>

#include "EspNow.hpp"
#include "Messages.hpp"
#include "mocks/Arduino8266AdpMock.hpp"
#include "mocks/Wifi8266AdpMock.hpp"
#include "mocks/Esp8266AdpMock.hpp"
#include "mocks/EspNow8266AdpMock.hpp"

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

    void setInitMocks()
    {
        mock("EspNow8266AdpMock").expectOneCall("init");
        mock("EspNow8266AdpMock")
            .expectOneCall("setRole")
            .withParameter("role", static_cast<int>(IEspNow8266Adp::Role::COMBO));
        mock("EspNow8266AdpMock").expectOneCall("registerOnRecvCb");
        mock("EspNow8266AdpMock").expectOneCall("registerOnSendCb");

        mock("Wifi8266AdpMock").expectOneCall("setModeSta");
        mock("Wifi8266AdpMock").expectOneCall("setChannel").withParameter("channel", 0);
        mock("Wifi8266AdpMock").expectOneCall("disconnect");
    }

    std::shared_ptr<IArduino8266Adp> arduinoAdp{std::make_shared<Arduino8266AdpMock>()};
    std::shared_ptr<IWifi8266Adp> wifiAdp{std::make_shared<Wifi8266AdpMock>()};
    std::shared_ptr<EspNow8266AdpMock> espNowAdp{std::make_shared<EspNow8266AdpMock>()};
    std::shared_ptr<Esp8266AdpMock> espAdp{std::make_shared<Esp8266AdpMock>()};
    constexpr static auto LED_BUILTIN = 9;
};

TEST(TestEspNow, ShouldInitEspNow)  // NOLINT
{
    EspNow espNow{arduinoAdp, wifiAdp, espAdp, espNowAdp};
    setInitMocks();
    espNow.init(0);
}

TEST(TestEspNow, ShouldAccepttPairResp)  // NOLINT
{
    EspNow espNow{arduinoAdp, wifiAdp, espAdp, espNowAdp};
    setInitMocks();

    mock("Wifi8266AdpMock").expectOneCall("macAddress");

    espNow.init(0);

    MacAddr mac = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};  // NOLINT
    auto msg = PairRespMsg::create();
    msg.channel = 1;
    msg.hostMacAddr = mac;
    msg.updatePeriodMins = 2;
    auto serializedMsg = msg.serialize();

    auto status = espNowAdp->testingCallOnRecv(mac, serializedMsg.data(), serializedMsg.size());
    CHECK_TRUE(status == IEspNow8266Adp::MsgHandleStatus::ACCEPTED);
}

TEST(TestEspNow, ShouldDoNothingOnNotSupportedMessages)  // NOLINT
{
    EspNow espNow{arduinoAdp, wifiAdp, espAdp, espNowAdp};
    setInitMocks();
    espNow.init(0);

    MacAddr mac = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};  // NOLINT

    auto msg1 = PairReqMsg::create();
    auto serializedMsg1 = msg1.serialize();
    auto status = espNowAdp->testingCallOnRecv(mac, serializedMsg1.data(), serializedMsg1.size());
    CHECK_TRUE(status == IEspNow8266Adp::MsgHandleStatus::REJECTED);

    auto msg2 = SensorDataMsg::create();
    auto serializedMsg2 = msg2.serialize();
    status = espNowAdp->testingCallOnRecv(mac, serializedMsg2.data(), serializedMsg2.size());
    CHECK_TRUE(status == IEspNow8266Adp::MsgHandleStatus::REJECTED);

    auto msg3 = SensorDataMsg::create();
    msg3.msgType = MsgType::UNKNOWN;
    auto serializedMsg3 = msg3.serialize();
    status = espNowAdp->testingCallOnRecv(mac, serializedMsg2.data(), serializedMsg2.size());
    CHECK_TRUE(status == IEspNow8266Adp::MsgHandleStatus::REJECTED);
}

TEST(TestEspNow, ShouldReturnNulloptIfCantPair)  // NOLINT
{
    EspNow espNow{arduinoAdp, wifiAdp, espAdp, espNowAdp};
    setInitMocks();
    espNow.init(0);

    constexpr auto maxChannelNum = 12;
    constexpr auto waitTime = 1000;

    for (int i = 0; i <= maxChannelNum; ++i)
    {
        mock("Wifi8266AdpMock").expectOneCall("setChannel").withParameter("channel", i);
        mock().ignoreOtherCalls();
    }

    CHECK_FALSE(espNow.pair());  // NOLINT
}

// TODO: NOT TESTABLE, NEEDS REDESIGN CLASS TO BE ABLE TO INJECT BETWEEN CALLS
// TEST(TestEspNow, ShouldSetupAfterPairMsg)  // NOLINT
// {
//     EspNow espNow{arduinoAdp, wifiAdp, espAdp, espNowAdp};
//     setInitMocks();
//     espNow.init(0);

//     auto onWaitCb = EspAdp::createAndInjectOnWaitCb(
//         [this, &espNow]
//         {
//             acceptPairRespMsg(espNow);
//         });
//     constexpr auto waitTime = 1000;

//     mock().expectOneCall("WiFiAdp::setChannel").withParameter("channel", 0);
//     mock().expectOneCall("EspNowAdp::send").andReturnValue(0);
//     mock().ignoreOtherCalls();

//     CHECK_TRUE(espNow.pair());  // NOLINT
// }
