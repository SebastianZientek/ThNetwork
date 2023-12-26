#include "MacAddr.hpp"

#include <cstring>

bool operator<(const MacAddr &lhs, const MacAddr &rhs)
{
    return lhs.addrData < rhs.addrData;
}

bool operator==(const MacAddr &lhs, const MacAddr &rhs)
{
    return lhs.addrData == rhs.addrData;
}

std::array<uint8_t, MacAddr::macAddrDigits> &MacAddr::arr()
{
    return addrData;
}

MacAddr MacAddr::strToMac(const std::string &str)
{
    MacAddr mac;
    std::sscanf(str.c_str(), "%x:%x:%x:%x:%x:%x", &mac.data()[0], &mac.data()[1], &mac.data()[2],
                &mac.data()[3], &mac.data()[4], &mac.data()[5]);

    return mac;
}

const uint8_t *MacAddr::data() const
{
    return addrData.data();
}

uint8_t *MacAddr::data()
{
    return addrData.data();
}

std::string MacAddr::str() const
{
    constexpr auto macStrWithNullSize = 18;
    std::array<char, macStrWithNullSize> macCStr{};
    snprintf(macCStr.data(), sizeof(macCStr), "%02x:%02x:%02x:%02x:%02x:%02x",  // NOLINT
             addrData[0],                                                       // NOLINT
             addrData[1], addrData[2], addrData[3], addrData[4], addrData[5]);  // NOLINT
    return macCStr.data();
}
