#pragma once

#include <array>
#include <cstdint>

#include "serializer.hpp"

enum class MsgType : uint8_t
{
    PAIR_REQ,
    PAIR_RESP,
    SENSOR_DATA,
    UNKNOWN
};

struct PairReqMsg
{
    MsgType msgType{MsgType::PAIR_REQ};
    std::array<uint8_t, 6> transmitterMacAddr{};

    static auto create()
    {
        return PairReqMsg({MsgType::PAIR_REQ, {}});
    }

    [[nodiscard]] auto serialize() const
    {
        return serializer::serialize(msgType, transmitterMacAddr);
    }

    void deserialize(const uint8_t *buffer, size_t size)
    {
        auto valuesOpt = serializer::deserialize<decltype(msgType), decltype(transmitterMacAddr)>(
            buffer, size);

        if (valuesOpt)
        {
            std::tie(msgType, transmitterMacAddr) = valuesOpt.value();
        }
    }
};

struct PairRespMsg
{
    MsgType msgType{MsgType::PAIR_RESP};
    std::array<uint8_t, 6> hostMacAddr{};
    uint8_t channel{};
    uint8_t updatePeriodMins{};

    template <typename... Ts>
    static auto create(Ts... ts)
    {
        return PairRespMsg({MsgType::PAIR_RESP, {}, ts...});
    }

    [[nodiscard]] auto serialize() const
    {
        return serializer::serialize(msgType, hostMacAddr, channel, updatePeriodMins);
    }

    void deserialize(const uint8_t *buffer, size_t size)
    {
        auto valuesOpt
            = serializer::deserialize<decltype(msgType), decltype(hostMacAddr), decltype(channel),
                                      decltype(updatePeriodMins)>(buffer, size);

        if (valuesOpt)
        {
            std::tie(msgType, hostMacAddr, channel, updatePeriodMins) = valuesOpt.value();
        }
    }
};

struct SensorDataMsg
{
    MsgType msgType{MsgType::SENSOR_DATA};
    std::array<uint8_t, 6> transmitterMacAddr{};
    float temperature{};
    float humidity{};

    template <typename... Ts>
    static auto create(Ts... ts)
    {
        return SensorDataMsg({MsgType::SENSOR_DATA, {}, ts...});
    }

    [[nodiscard]] auto serialize() const
    {
        return serializer::serialize(msgType, transmitterMacAddr, temperature, humidity);
    }

    void deserialize(const uint8_t *buffer, size_t size)
    {
        auto valuesOpt
            = serializer::deserialize<decltype(msgType), decltype(transmitterMacAddr),
                                      decltype(temperature), decltype(humidity)>(buffer, size);

        if (valuesOpt)
        {
            std::tie(msgType, transmitterMacAddr, temperature, humidity) = valuesOpt.value();
        }
    }
};
