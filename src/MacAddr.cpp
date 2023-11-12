#include "MacAddr.hpp"

#include <cstring>

MacAddr::MacAddr(const uint8_t *mac) { std::memcpy(m_macAddr.data(), mac, macDigits); }

MacAddr::operator String()
{
    constexpr auto macStrWithNullSize = 18;
    std::array<char, macStrWithNullSize> macCStr{};
    snprintf(macCStr.data(), sizeof(macCStr), "%02x:%02x:%02x:%02x:%02x:%02x", m_macAddr[0], m_macAddr[1],
             m_macAddr[2], m_macAddr[3], m_macAddr[4], m_macAddr[5]);
    return macCStr.data();
}

String MacAddr::str() { return operator String(); }

MacAddr::operator uint8_t *() { return m_macAddr.data(); }

bool operator<(const MacAddr &lhs, const MacAddr &rhs) { return lhs.m_macAddr < rhs.m_macAddr; }
