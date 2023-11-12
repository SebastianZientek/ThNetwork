#pragma once

#include <Arduino.h>

#include <array>
#include <cstdint>

class MacAddr
{
public:
    MacAddr(const uint8_t *mac);
    String str();
    operator String();
    operator uint8_t *();

    friend bool operator<(const MacAddr &lhs, const MacAddr &rhs);

private:
    constexpr static auto macDigits = 6;
    std::array<uint8_t, macDigits> m_macAddr{};
};
