#pragma once

#include <array>
#include <cstdint>

#include "MacAddr.hpp"
#include "serializer.hpp"

enum class MsgType : uint8_t
{
    PAIR_REQ,
    PAIR_RESP,
    SENSOR_DATA,
    UNKNOWN
};

using Signature = std::array<char, 4>;
constexpr Signature signatureTemplate{'T', 'H', 'T', 'H'};

struct PairReqMsg
{
    MsgType msgType;
    Signature signature;
    MacAddr transmitterMacAddr;

    static auto create()
    {
        return PairReqMsg({MsgType::PAIR_REQ, signatureTemplate, {}});
    }

    [[nodiscard]] auto serialize() const
    {
        return serializer::serialize(msgType, signature, transmitterMacAddr);
    }

    void deserialize(const uint8_t *buffer, size_t size)
    {
        auto valuesOpt = serializer::deserialize<decltype(msgType), decltype(signature),
                                                 decltype(transmitterMacAddr)>(buffer, size);

        if (valuesOpt)
        {
            std::tie(msgType, signature, transmitterMacAddr) = valuesOpt.value();
        }
    }
};

struct PairRespMsg
{
    MsgType msgType;
    Signature signature;
    MacAddr hostMacAddr;
    uint8_t channel;
    uint8_t updatePeriodMins;

    template <typename... Ts>
    static auto create(Ts... args)
    {
        return PairRespMsg({MsgType::PAIR_RESP, signatureTemplate, {}, args...});
    }

    [[nodiscard]] auto serialize() const
    {
        return serializer::serialize(msgType, signature, hostMacAddr, channel, updatePeriodMins);
    }

    void deserialize(const uint8_t *buffer, size_t size)
    {
        auto valuesOpt
            = serializer::deserialize<decltype(msgType), decltype(signature), decltype(hostMacAddr),
                                      decltype(channel), decltype(updatePeriodMins)>(buffer, size);

        if (valuesOpt)
        {
            std::tie(msgType, signature, hostMacAddr, channel, updatePeriodMins)
                = valuesOpt.value();
        }
    }
};

struct SensorDataMsg
{
    MsgType msgType;
    Signature signature;
    MacAddr transmitterMacAddr;
    float temperature;
    float humidity;

    template <typename... Ts>
    static auto create(Ts... args)
    {
        return SensorDataMsg({MsgType::SENSOR_DATA, signatureTemplate, {}, args...});
    }

    [[nodiscard]] auto serialize() const
    {
        return serializer::serialize(msgType, signature, transmitterMacAddr, temperature, humidity);
    }

    void deserialize(const uint8_t *buffer, size_t size)
    {
        auto valuesOpt
            = serializer::deserialize<decltype(msgType), decltype(signature),
                                      decltype(transmitterMacAddr), decltype(temperature),
                                      decltype(humidity)>(buffer, size);

        if (valuesOpt)
        {
            std::tie(msgType, signature, transmitterMacAddr, temperature, humidity)
                = valuesOpt.value();
        }
    }
};
