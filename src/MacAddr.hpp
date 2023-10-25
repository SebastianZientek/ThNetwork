#pragma once

#include <Arduino.h>

#include <array>
#include <cstdint>

class MacAddr
{
public:
    MacAddr(const uint8_t *mac);
    operator String();
    operator uint8_t *();

private:
    constexpr static auto macDigits = 6;
    std::array<uint8_t, macDigits> macAddr{};
    String macString;
};