#include "MacAddr.hpp"

#include <cstring>

MacAddr::MacAddr(const uint8_t *mac)
    : m_macString()
{
    std::memcpy(m_macAddr.data(), mac, macDigits);
}

MacAddr::operator String()
{
    if (m_macString.isEmpty())
    {
        char macCStr[18];  // NOLINT
        snprintf(macCStr, sizeof(macCStr), "%02x:%02x:%02x:%02x:%02x:%02x", m_macAddr[0],
                 m_macAddr[1], m_macAddr[2], m_macAddr[3], m_macAddr[4], m_macAddr[5]);
        m_macString = &macCStr[0];
    }
    return m_macString;
}

String MacAddr::str() { return operator String(); }

MacAddr::operator uint8_t *() { return m_macAddr.data(); }

bool operator<(const MacAddr &lhs, const MacAddr &rhs) { return lhs.m_macString < rhs.m_macString; }
