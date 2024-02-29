#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>

struct MacAddr
{
    constexpr static auto macAddrDigits = 6;
    std::array<uint8_t, macAddrDigits> addrData;

    std::array<uint8_t, macAddrDigits> &arr();
    [[nodiscard]] const uint8_t *data() const;
    [[nodiscard]] uint8_t *data();
    [[nodiscard]] std::string str() const;
    [[nodiscard]] size_t toUniqueID() const;

    friend bool operator<(const MacAddr &lhs, const MacAddr &rhs);
    friend bool operator==(const MacAddr &lhs, const MacAddr &rhs);
};

static_assert(std::is_trivial_v<MacAddr>);
