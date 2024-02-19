#include <functional>

#include "adapters/IEspNow8266Adp.hpp"
#include "common/MacAddr.hpp"

class EspNow8266AdpMock : public IEspNow8266Adp
{
public:
    Status init() override
    {
        static auto defaultState = Status::OK;
        mock("EspNow8266AdpMock").actualCall("init");
        auto *returnVal = mock("EspNow8266AdpMock").returnPointerValueOrDefault(&defaultState);

        return *static_cast<Status *>(returnVal);
    }

    void deinit() override
    {
        mock("EspNow8266AdpMock").actualCall("deinit");
    }

    void setRole(Role role) override
    {
        mock("EspNow8266AdpMock")
            .actualCall("setRole")
            .withParameter("role", static_cast<int>(role));
    }

    void registerOnSendCb(const OnSendCb &onSendCb) override
    {
        m_onSendCb = onSendCb;
        mock("EspNow8266AdpMock").actualCall("registerOnSendCb");
    }

    void registerOnRecvCb(const OnRecvCb &onRecvCb) override
    {
        m_onRecvCb = onRecvCb;
        mock("EspNow8266AdpMock").actualCall("registerOnRecvCb");
    }

    Status sendData(MacAddr &mac, uint8_t *data, size_t length) override
    {
        static auto defaultState = Status::OK;
        auto retVal = mock("EspNow8266AdpMock")
                          .actualCall("sendData")
                          .withParameter("data", data)
                          .withParameter("length", length)
                          .returnPointerValueOrDefault(&defaultState);

        return *static_cast<Status *>(retVal);
    }

    void testingCallOnSend(const MacAddr &mac, Status status)
    {
        m_onSendCb(mac, status);
    }

    IEspNow8266Adp::MsgHandleStatus testingCallOnRecv(const MacAddr &mac, const uint8_t *incomingData, uint8_t len)
    {
        return m_onRecvCb(mac, incomingData, len);
    }

private:
    OnSendCb m_onSendCb;
    OnRecvCb m_onRecvCb;
};
