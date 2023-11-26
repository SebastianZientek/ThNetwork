#pragma once

#include <array>
#include <cinttypes>
#include <type_traits>

#include "common/MacAddr.hpp"

namespace config
{
// 4 byte alignment is required by RTC memory
struct alignas(4) TransmitterConfig
{
    MacAddr targetMac{};
    uint8_t channel = 0;
    uint8_t sensorUpdatePeriodMins = 0;
};

void saveToEEPROM(TransmitterConfig configuration);
TransmitterConfig readFromEEPROM();

void saveToRTC(TransmitterConfig configuration);
TransmitterConfig readFromRTC();
}  // namespace config
