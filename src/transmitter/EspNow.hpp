#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "adapters/IArduino8266Adp.hpp"
#include "adapters/IEsp8266Adp.hpp"
#include "adapters/IEspNow8266Adp.hpp"
#include "adapters/IWifi8266Adp.hpp"
#include "common/MacAddr.hpp"
#include "common/Messages.hpp"
#include "common/serializer.hpp"
#include "config.hpp"

class EspNow
{
public:
    EspNow(const std::shared_ptr<IArduino8266Adp> &arduinoAdp,
           const std::shared_ptr<IWifi8266Adp> &wifiAdp,
           const std::shared_ptr<IEsp8266Adp> &espAdp,
           const std::shared_ptr<IEspNow8266Adp> &espNowAdp);
    ~EspNow() = default;
    EspNow(const EspNow &) = delete;
    EspNow(EspNow &&) = delete;
    EspNow &operator=(const EspNow &) = delete;
    EspNow &operator=(EspNow &&) = delete;

    void init(uint8_t channel = 0);
    std::optional<config::TransmitterConfig> pair();
    void sendDataToHost(std::size_t identifier, MacAddr mac, float temperature, float humidity);
    [[nodiscard]] config::TransmitterConfig getTransmitterConfig() const;

private:
    constexpr static std::array<uint8_t, 4> msgSignature{'T', 'H', 'D', 'T'};

    std::shared_ptr<IArduino8266Adp> m_arduinoAdp;
    std::shared_ptr<IWifi8266Adp> m_wifiAdp;
    std::shared_ptr<IEsp8266Adp> m_espAdp;
    std::shared_ptr<IEspNow8266Adp> m_espNowAdp;

    bool m_paired{false};
    config::TransmitterConfig m_transmitterConfig{};

    IEspNow8266Adp::MsgHandleStatus onDataRecv(const MacAddr &mac,
                                               const uint8_t *incomingData,
                                               int len);
    void onDataSend(const MacAddr &mac, IEspNow8266Adp::Status status);
    void setOnDataRecvCb();
    void setOnDataSendCb();
    void sendPairMsg();

    [[nodiscard]] MsgType getMsgType(const uint8_t *buffer, size_t size) const;
};
