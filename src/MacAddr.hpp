#pragma once

#include <Arduino.h>

#include <array>
#include <cstdint>

class MacAddr
{
public:
    MacAddr(const uint8_t *mac);
    String str()
    {
        return operator String();
    }
    
    operator String();
    operator uint8_t *();
    friend bool operator<(const MacAddr& lhs, const MacAddr& rhs)
    {
        return lhs.macString < rhs.macString;
    }


private:
    constexpr static auto macDigits = 6;
    std::array<uint8_t, macDigits> macAddr{};
    String macString;
};
