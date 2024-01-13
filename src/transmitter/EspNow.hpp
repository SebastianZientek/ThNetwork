#pragma once

#include <cstdint>
#include <optional>

#include "common/MacAddr.hpp"
#include "common/Messages.hpp"
#include "common/serializer.hpp"
#include "config.hpp"

class EspNow
{
public:
    EspNow() = default;
    ~EspNow() = default;
    EspNow(const EspNow &) = delete;
    EspNow(EspNow &&) = delete;
    EspNow &operator=(const EspNow &) = delete;
    EspNow &operator=(EspNow &&) = delete;

    void init(uint8_t channel = 0);
    std::optional<config::TransmitterConfig> pair();
    void sendDataToHost(std::size_t ID, MacAddr mac, float temperature, float humidity);
    config::TransmitterConfig getTransmitterConfig();

private:
    constexpr static std::array<uint8_t, 4> msgSignature{'T', 'H', 'D', 'T'};
    bool m_paired{false};
    config::TransmitterConfig m_transmitterConfig{};

    using OnSendCb = std::function<void(const MacAddr &mac, uint8_t status)>;
    using OnRecvCb
        = std::function<void(const MacAddr &mac, const uint8_t *incomingData, uint8_t len)>;

    static OnSendCb m_onSend;  // NOLINT
    static OnRecvCb m_onRecv;  // NOLINT

    void onDataRecv(const MacAddr &mac, const uint8_t *incomingData, int len);
    void onDataSend(const MacAddr &mac, uint8_t status);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void sendPairMsg();

    MsgType getMsgType(const uint8_t *buffer, size_t size);
};
